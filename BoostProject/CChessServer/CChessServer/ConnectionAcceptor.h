#ifndef TCPCONNECTIONSERVER_H
#define TCPCONNECTIONSERVER_H
#include <boost/asio.hpp>
#include "Session.h"

class ConnectionAcceptor
{
public:
	explicit ConnectionAcceptor(boost::asio::io_service& io_service);
	~ConnectionAcceptor();

public:
	void start_accept();

private:
	bool init();
	void handle_accept(Session::sessionPtr,const boost::system::error_code& error);

private:
	std::vector<Session::sessionPtr> m_sessionList;
	tcp::acceptor acceptor_;
};

#endif