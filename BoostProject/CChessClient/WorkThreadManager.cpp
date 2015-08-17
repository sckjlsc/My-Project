#include "WorkThreadManager.h"

WorkThreadManager::WorkThreadManager(ControlerManager* mgr, QObject* parent /* = NULL */)
	: QObject(parent), m_networkThread(NULL), m_controlerMgr(mgr)
{
}

WorkThreadManager::~WorkThreadManager()
{
	if (m_networkThread != NULL)
    {
		m_networkThread->exit();
		m_networkThread->deleteLater();
    }
}

bool WorkThreadManager::initThreads()
{
	m_networkThread = new QThread();
	m_networkThread->start();
    return true;
}

bool WorkThreadManager::startThreads()
{
	//m_networkThread->start();
    return true;
}

QThread* WorkThreadManager::mainThread() const
{
    return qApp->thread();
}
QThread* WorkThreadManager::guiThread() const
{
    return qApp->thread();
}
