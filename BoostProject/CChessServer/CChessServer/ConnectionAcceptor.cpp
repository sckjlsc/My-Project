#include "stdafx.h"
#include "ConnectionAcceptor.h"


ConnectionAcceptor::ConnectionAcceptor(boost::asio::io_service& io_service)
		: acceptor_(io_service, tcp::endpoint(tcp::v4(), 6688))
{
	init();
}

ConnectionAcceptor::~ConnectionAcceptor()
{

}

bool ConnectionAcceptor::init()
{
	return true;
}

void ConnectionAcceptor::start_accept()
{
	Session::sessionPtr new_connection = Session::createSession(acceptor_.get_io_service());
	m_sessionList.push_back(new_connection);
	acceptor_.async_accept(new_connection->sock(),
	boost::bind(&ConnectionAcceptor::handle_accept, this, new_connection,
	boost::asio::placeholders::error));
}

void ConnectionAcceptor::handle_accept(Session::sessionPtr new_connection, const boost::system::error_code& error)
{
	if (!error)
	{
		new_connection->start();
	}

	start_accept();
}

