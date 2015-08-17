#include "Client.h"
#include <boost/date_time/posix_time/posix_time.hpp>  
#include "ControlerManager.h"
#include "NetworkManager.h"

#define MEM_FN(x)       boost::bind(&self_type::x, this)
#define MEM_FN1(x,y)    boost::bind(&self_type::x, this,y)
#define MEM_FN2(x,y,z)  boost::bind(&self_type::x, this,y,z)
//the same meaning "sock_.async_connect(ep, MEM_FN1(on_connect,_1));"
//sock_.async_connect(ep, boost::bind(&talk_to_svr::on_connect, shared_ptr_from_this(), _1));
//sock_.async_connect(ep, boost::bind(&talk_to_svr::on_connect, this, _1));
Client::Client(boost::asio::io_service& svc, QObject* parent)
	:QObject(parent), m_ioService(svc), m_socket(m_ioService)
{
	//m_socketPtr = socket_ptr(new ip::tcp::socket(m_ioService));
}
Client::~Client()
{

}

bool Client::init()
{
	return true;
}
bool Client::connectSigSlot()
{
	connect(this, SIGNAL(responseReceived(const QString&)), atpControlers->networkManager(), SLOT(onResponseReceived(const QString&)));
	return true;
}

bool Client::connectToServer()
{
	ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 6688);
	try
	{
		m_socket.async_connect(ep, bind(&Client::conn_handler, this, boost::asio::placeholders::error));
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return true;
}
void Client::conn_handler(const boost::system::error_code& ec)
{
	if (ec)
	{
		return;
	}
	boost::asio::socket_base::keep_alive option(true);
	m_socket.set_option(option);
	do_read();
	do_write("client 0");
	//do_write("connected to " + m_socket.remote_endpoint().address().to_string() + "\n");  
}

void Client::do_read()
{
	memset(read_buffer_, 0, sizeof(read_buffer_));
	async_read(m_socket, buffer(read_buffer_), MEM_FN2(is_read_complete, _1, _2), MEM_FN2(handle_read, _1, _2));
}
size_t Client::is_read_complete(const boost::system::error_code &err, size_t bytes)
{
	if (err)
	{
		return 0;
	}
	bool found = std::find(read_buffer_, read_buffer_ + bytes, '\n') < read_buffer_ + bytes;
	if (bytes == 8)
	{
		return 0;
	}
	return 1;

	
	return found ? 0 : 1;
}
void Client::handle_read(const boost::system::error_code & err, size_t bytes)
{
	if (err)
	{
		return;
	}
	std::string msg(read_buffer_, bytes);
	QString replyReceivedStr = QString::fromStdString(msg);
	emit responseReceived(replyReceivedStr);
	do_read();
}
void Client::do_write(const std::string& msg)
{
	memset(write_buffer_, 0, sizeof(write_buffer_));
	std::copy(msg.begin(), msg.end(), write_buffer_);
	m_socket.async_write_some(buffer(write_buffer_, msg.size()), MEM_FN2(handle_write, _1, _2));
}
void Client::handle_write(const boost::system::error_code& err, size_t /*bytes_transferred*/)
{
	if (err)
	{
		return;
	}
}

void Client::send(const QString& message)
{
	std::string msg = message.toStdString();
	do_write(msg);
}



