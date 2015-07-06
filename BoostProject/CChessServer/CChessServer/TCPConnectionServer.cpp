#include "stdafx.h"
#include "TCPConnectionServer.h"


TCPConnectionServer::TCPConnectionServer(boost::asio::io_service& io_service)
		: acceptor_(io_service, tcp::endpoint(tcp::v4(), 6688))
{
	start_accept();
}

TCPConnectionServer::~TCPConnectionServer()
{

}

void TCPConnectionServer::start_accept()
{
	Session::sessionPtr new_connection = Session::create(acceptor_.get_io_service());
	m_sessionList.push_back(new_connection);
	acceptor_.async_accept(new_connection->socket(),
	boost::bind(&TCPConnectionServer::handle_accept, this, new_connection,
	boost::asio::placeholders::error));
}

void TCPConnectionServer::handle_accept(Session::sessionPtr new_connection, const boost::system::error_code& error)
{
	if (!error)
	{
		new_connection->start();
	}

	start_accept();
}

