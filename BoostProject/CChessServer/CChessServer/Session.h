#ifndef SESSION_H
#define SESSION_H

#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
using boost::asio::ip::tcp;
using namespace std;

class Session : public boost::enable_shared_from_this <Session>, boost::noncopyable
{
public:
	typedef boost::shared_ptr<Session> sessionPtr;
	typedef Session self_type;
	static sessionPtr create(boost::asio::io_service& io_service)
	{
		return sessionPtr(new Session(io_service));
	}

	tcp::socket& socket()
	{
		return socket_;
	}

	void start();
	
private:
	Session(boost::asio::io_service& io_service);
	void do_write(const std::string& msgToWrite);
	void handle_write(const boost::system::error_code& /*error*/,size_t /*bytes_transferred*/);
	
private:
	void do_read();
	void handle_read(const boost::system::error_code & err, size_t bytes);
	size_t is_read_complete(const boost::system::error_code &err, size_t bytes);
	void handle_read_until(const boost::system::error_code& err);
private:
	tcp::socket socket_;

	enum { max_msg = 10240 };
	char read_buffer_[max_msg];
	char write_buffer_[max_msg];
	boost::asio::streambuf response_;
};

#endif