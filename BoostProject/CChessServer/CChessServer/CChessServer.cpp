#include "stdafx.h"

#include <boost/asio.hpp>
#include "Session.h"
#include "ConnectionAcceptor.h"

boost::asio::io_service service;

int main(int argc, char* argv[]) 
{
	ConnectionAcceptor conAcceptor(service);
	conAcceptor.start_accept();

	service.run();
}