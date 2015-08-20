#ifndef WORKTHREADMANAGER_H
#define WORKTHREADMANAGER_H

#include <QObject>
#include <QSharedPointer>
#include <QThread>
#include <QCoreApplication>
class ControlerManager;
class WorkThreadManager : public QObject
{
    Q_OBJECT
public: // ctor & dtor
	explicit WorkThreadManager(ControlerManager* mgr, QObject* parent = NULL);
    ~WorkThreadManager();

public: // fundamental method
    bool initThreads();
    bool startThreads();

public: // accessor
    QThread* guiThread() const;
    QThread* mainThread() const;

    // specially for recving & sending network data
    QThread* NetworkThread() const { return m_networkThread; }

private:
    QThread* m_networkThread;
	ControlerManager* m_controlerMgr;
};


#endif // WORKTHREADMANAGER_H
