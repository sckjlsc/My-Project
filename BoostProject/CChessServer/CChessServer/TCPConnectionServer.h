#ifndef TCPCONNECTIONSERVER_H
#define TCPCONNECTIONSERVER_H
#include <boost/asio.hpp>

#include "Session.h"

class TCPConnectionServer
{
public:
	explicit TCPConnectionServer(boost::asio::io_service& io_service);
	~TCPConnectionServer();

private:
	void start_accept();
	void handle_accept(Session::sessionPtr,const boost::system::error_code& error);
	std::vector<Session::sessionPtr> m_sessionList;
	tcp::acceptor acceptor_;
};

#endif