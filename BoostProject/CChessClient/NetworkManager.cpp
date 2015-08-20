#include "NetworkManager.h"
#include "Client.h"
#include "ControlerManager.h"

NetworkManager::NetworkManager(ControlerManager* mgr, QObject* parent)
	:QObject(parent), m_controlerMgr(mgr)
{
	m_client = new Client(service);
}

bool NetworkManager::init()
{
	m_client->init();
	return true;
}
bool NetworkManager::connectSigSlot()
{
	m_client->connectSigSlot();
	return true;
}

bool NetworkManager::startup()
{
	m_thread = new boost::thread([this]{service.run();});
	//thread1.join();
	return true;
}

void NetworkManager::onStartConnect()
{	
	m_client->connectToServer();	
	startup();
}

void NetworkManager::sendMsg(const QString& msg)
{
	m_client->send(msg);
}

void NetworkManager::onResponseReceived(const QString& res)
{
	emit messageReceived(res);
}