#include "stdafx.h"
#include "Session.h"

#define MEM_FN(x)       boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y)    boost::bind(&self_type::x, shared_from_this(), y)
#define MEM_FN2(x,y,z)  boost::bind(&self_type::x, shared_from_this(), y, z)

using namespace boost::asio;

Session::Session(boost::asio::io_service& io_service)
: socket_(io_service)
{
}
void Session::start()
{
	do_write("connected!");
}
void Session::do_read()
{
	//boost::shared_ptr<vector<char> > str(new vector<char>(100, 0));
	//socket_.async_read_some(buffer(*str), bind(&Session::handle_read, this, boost::asio::placeholders::error, str));
	//async_read(socket_, buffer(*str), MEM_FN2(read_complete, _1, _2), MEM_FN2(on_read, _1, _2));
	//async_read(socket_, buffer(read_buffer_), MEM_FN2(is_read_complete, _1, _2), MEM_FN2(handle_read, _1, _2));
	boost::asio::async_read_until(socket_, response_, "\n",
		boost::bind(&Session::handle_read_until, this,
		boost::asio::placeholders::error));
}
size_t Session::is_read_complete(const boost::system::error_code &err, size_t bytes)
{
	if (err) return 0;
	if (bytes == 0) return 0;
	bool found = (std::find(read_buffer_, read_buffer_ + bytes, '\n') < read_buffer_ + bytes);
	return found ? 0 : 1;
}
void Session::handle_read(const boost::system::error_code & err, size_t bytes) 
{
	if (bytes == 0)
	{
		return;
	}
	std::string msg(read_buffer_, bytes);
	std::string replyMsg = "reply " + msg;
	do_write(replyMsg);

}
void Session::do_write(const std::string& msgToWrite)
{
	boost::asio::async_write(socket_, boost::asio::buffer(msgToWrite),
		boost::bind(&Session::handle_write, shared_from_this(),
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));
	do_read();
}
void Session::handle_write(const boost::system::error_code& /*error*/,size_t /*bytes_transferred*/)
{
	//do_read();
	//boost::shared_ptr<vector<char> > str(new vector<char>(100, 0));
	//socket_.async_read_some(buffer(*str), bind(&Session::handle_read, this, boost::asio::placeholders::error, str));
	//async_read(sock_, buffer(read_buffer_), MEM_FN2(read_complete, _1, _2), MEM_FN2(on_read, _1, _2));
}

void Session::handle_read_until(const boost::system::error_code& err)
{
	std::istream response_stream(&response_);
	std::string status_message;
	std::getline(response_stream, status_message);
}