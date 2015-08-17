#include "stdafx.h"
#include "Session.h"

Session::sessionPtr Session::createSession(boost::asio::io_service& service) {
	sessionPtr newSessionPtr(new Session(service));
	return newSessionPtr;
}

void Session::start()
{
	started_ = true;
	boost::asio::socket_base::keep_alive option(true);
	sock_.set_option(option);
	do_read_header();
}

void Session::stop() {
	if (!started_) return;
	started_ = false;
	sock_.close();
}

void Session::do_read_header()
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

void Session::do_read_body(int body_length)
{
	memset(read_buffer_, 0, sizeof(read_buffer_));
	async_read(sock_, buffer(read_buffer_, body_length), MEM_FN2(on_read, _1, _2));
}

void Session::do_write(const std::string & msg) {
	if (!started()) return;
	memset(&write_buffer_[0], 0, sizeof(write_buffer_));
	char header[header_len + 1] = "";
	std::sprintf(header, "%4d", msg.length());
	std::memcpy(write_buffer_, header, header_len);
	std::strcpy(write_buffer_ + header_len, msg.c_str());
	//std::copy(msg.begin(), msg.end(), (write_buffer_ + header_len));
	sock_.async_write_some(buffer(write_buffer_, msg.size() + header_len), MEM_FN2(on_write, _1, _2));
}

size_t Session::read_complete(const boost::system::error_code & err, size_t bytes) {
	if (err) return 0;
	if (bytes == 8)
	{
		return 0;
	}
	return 1;
	bool found = std::find(read_buffer_, read_buffer_ + bytes, '\n') < read_buffer_ + bytes;
	return found ? 0 : 1;
}

void Session::on_read(const boost::system::error_code & err, size_t bytes) {
	if (!err) {
		std::string msg(read_buffer_, bytes);
		do_write(msg);//+"\n");
	}
	//stop();
}

void Session::on_write(const boost::system::error_code & err, size_t bytes) {
	do_read_header();
}