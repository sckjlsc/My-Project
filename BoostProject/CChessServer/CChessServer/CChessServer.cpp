#include "stdafx.h"
#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "TCPConnectionServer.h"
#include <boost/thread.hpp>
//boost::asio::io_service io_service;
//void runService1()
//{
//	io_service.run();
//}
//
//int main()
//{
//	try
//	{
//		TCPConnectionServer server(io_service);
//		boost::thread thread1(runService1);
//		thread1.join();
//	}
//	catch (std::exception& e)
//	{
//		std::cerr << e.what() << std::endl;
//	}
//
//	return 0;
//}

//using boost::asio::ip::tcp;

using namespace std;
using namespace boost::asio;
boost::asio::io_service service;
#define MEM_FN(x)       boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y)    boost::bind(&self_type::x, shared_from_this(), y)
#define MEM_FN2(x,y,z)  boost::bind(&self_type::x, shared_from_this(), y, z)

class talk_to_client : public boost::enable_shared_from_this < talk_to_client > , boost::noncopyable {
	typedef talk_to_client self_type;
	talk_to_client() : sock_(service), started_(false) {}
public:
	typedef boost::system::error_code error_code;
	typedef boost::shared_ptr<talk_to_client> ptr;
	void start() {
		started_ = true;
		boost::asio::socket_base::keep_alive option(true);
		sock_.set_option(option);
		do_read_header();
	}

	static ptr new_() {
		ptr new_(new talk_to_client);
		return new_;
	}
	void stop() {
		if (!started_) return;
		started_ = false;
		sock_.close();
	}
	ip::tcp::socket & sock() { return sock_; }
	//void do_read() {
	//	memset(&read_buffer_[0], 0, sizeof(read_buffer_));
	//	async_read(sock_, buffer(read_buffer_), MEM_FN2(read_complete, _1, _2), MEM_FN2(on_read, _1, _2));
	//}
	void do_read_header()
	{
		memset(read_buffer_, 0, sizeof(read_buffer_));
		async_read(sock_, buffer(read_buffer_, header_len),
			[this](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				char header[header_len + 1] = "";
				std::strncat(header, read_buffer_, header_len);
				int body_length = std::atoi(header);


				do_read_body(body_length);
			}
		});
	}

	void do_read_body(int body_length)
	{
		memset(read_buffer_, 0, sizeof(read_buffer_));
		async_read(sock_, buffer(read_buffer_, body_length), MEM_FN2(on_read, _1, _2));
	}

	void do_write(const std::string & msg) {
		if (!started()) return;
		memset(&write_buffer_[0], 0, sizeof(write_buffer_));
		char header[header_len + 1] = "";
		std::sprintf(header, "%4d", msg.length());
		std::memcpy(write_buffer_, header, header_len);
		std::strcpy(write_buffer_ + header_len, msg.c_str());
		//std::copy(msg.begin(), msg.end(), (write_buffer_ + header_len));
		sock_.async_write_some(buffer(write_buffer_, msg.size() + header_len), MEM_FN2(on_write, _1, _2));
	}
	size_t read_complete(const boost::system::error_code & err, size_t bytes) {
		if (err) return 0;
		if (bytes == 8)
		{
			return 0;
		}
		return 1;
		bool found = std::find(read_buffer_, read_buffer_ + bytes, '\n') < read_buffer_ + bytes;
		return found ? 0 : 1;
	}
	void on_read(const boost::system::error_code & err, size_t bytes) {
		if (!err) {
			std::string msg(read_buffer_, bytes);
			do_write(msg);//+"\n");
		}
		//stop();
	}
	void on_write(const boost::system::error_code & err, size_t bytes) {
		do_read_header();
	}
	bool started() { return started_; }
private:
	ip::tcp::socket sock_;
	enum { header_len = 4 };
	enum { max_body_len = 65535 };
	char read_buffer_[max_body_len + header_len];
	char write_buffer_[max_body_len + header_len];
	bool started_;
};


ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 6688));
void handle_accept(talk_to_client::ptr client, const boost::system::error_code & err)
{
	client->start();
	talk_to_client::ptr new_client = talk_to_client::new_();
	acceptor.async_accept(new_client->sock(), boost::bind(handle_accept, new_client, _1));
}
int main(int argc, char* argv[]) {
	talk_to_client::ptr client = talk_to_client::new_();
	acceptor.async_accept(client->sock(), boost::bind(handle_accept, client, _1));
	service.run();
}