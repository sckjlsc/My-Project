#ifndef Client_H
#define Client_H


#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>  
#include <QObject>
//#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
//using boost::asio::ip::tcp;

using namespace std;
using namespace boost::asio;

//typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;

class Client: public QObject/*,public boost::enable_shared_from_this<Client>*/, boost::noncopyable
{
	Q_OBJECT
public:
	typedef Client self_type;

	explicit Client(boost::asio::io_service& svc,QObject* parent = NULL);
	~Client();
	bool init();
	bool connectSigSlot();
public:
	void send(const QString&);

signals:
	void responseReceived(const QString&);

public slots :
	bool connectToServer();

private:
	void conn_handler(const boost::system::error_code& ec);

private:
	void do_write(const std::string& msgToWrite);
	void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/);

private:
	void do_read_header();
	void do_read_body(int bodylen);
	void handle_read(const boost::system::error_code & err, size_t bytes);
	size_t is_read_complete(const boost::system::error_code &err, size_t bytes);

private:
	boost::asio::io_service& m_ioService;
	ip::tcp::socket m_socket;
	//socket_ptr m_socketPtr;
	enum { header_len = 4 };
	enum { max_body_len = 65535 };
	char read_buffer_[max_body_len + header_len];
	char write_buffer_[max_body_len + header_len];
};
#endif