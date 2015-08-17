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
using namespace boost::asio;

#define MEM_FN(x)       boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y)    boost::bind(&self_type::x, shared_from_this(), y)
#define MEM_FN2(x,y,z)  boost::bind(&self_type::x, shared_from_this(), y, z)

class Session : public boost::enable_shared_from_this < Session >, boost::noncopyable 
{
	typedef Session self_type;
	Session(boost::asio::io_service& service) : sock_(service), started_(false) {}
public:
	typedef boost::system::error_code error_code;
	typedef boost::shared_ptr<Session> sessionPtr;

	static sessionPtr createSession(boost::asio::io_service& service);

	ip::tcp::socket & sock() { return sock_; }
	bool started() { return started_; }

	void start();
	void stop();

	void do_read_header();
	void do_read_body(int body_length);
	void do_write(const std::string & msg);
	size_t read_complete(const boost::system::error_code & err, size_t bytes);
	void on_read(const boost::system::error_code & err, size_t bytes);
	void on_write(const boost::system::error_code & err, size_t bytes);
	
private:
	ip::tcp::socket sock_;
	enum { header_len = 4 };
	enum { max_body_len = 65535 };
	char read_buffer_[max_body_len + header_len];
	char write_buffer_[max_body_len + header_len];
	bool started_;
};

#endif