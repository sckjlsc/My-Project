#include <QtGlobal>
#include <QObject>
#include <QApplication>
#include <QResource>
#include <QFile>
#include <QTime>
#include "ControlerManager.h"
#include "NetworkManager.h"

ControlerManager* ControlerManager::self_ = NULL;

ControlerManager::ControlerManager(int &argc, char **argv)
{
    Q_ASSERT_X( (self_ == NULL), "ControlerManager", "Should only have 1 CpcCoreApp!");
    self_ = this;


    exiting_ = false;

    m_networkManager = new NetworkManager(this);
}

bool ControlerManager::init()
{
    bool succeeded = true;

    succeeded = m_networkManager->init();

    return succeeded;
}

bool ControlerManager::connectSigSlot()
{
	bool succeeded = true;

    succeeded = succeeded && m_networkManager->connectSigSlot();

    return succeeded;
}

bool ControlerManager::runProgram()
{
	bool succeeded = true;
	//succeeded = succeeded && m_networkManager->startup();
	return succeeded;
}

bool ControlerManager::exitProgram()
{
    //to do necessary un-init, db close.
    QApplication::closeAllWindows();
    QCoreApplication::quit();
    return true;
}
