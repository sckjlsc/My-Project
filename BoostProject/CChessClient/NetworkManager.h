#ifndef NetworkManager_H
#define NetworkManager_H

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <QObject>
#include <boost/thread.hpp>

using boost::asio::ip::tcp;
class Client;
class ControlerManager;

class NetworkManager:public QObject
{
	Q_OBJECT
public:
	explicit NetworkManager(ControlerManager* mgr, QObject* parent = NULL);

public:
	bool init();
	bool connectSigSlot();

	bool startup();
	void sendMsg(const QString& msg);

signals:
	void messageReceived(const QString&);

public slots :
	void onStartConnect();
	void onResponseReceived(const QString&);

private:
	Client* m_client;
	boost::thread *m_thread;
	ControlerManager* m_controlerMgr;
	boost::asio::io_service service;
	
};

#endif // NetworkManager_H