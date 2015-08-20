#include <qt/state/ControllerAccessor.hpp>
#include <qt/state/LoginController.hpp>
#include <qt/state/ControllerAccessor.hpp>
#include <qt/state/ActorClient.hpp>

#include <QLocalSocket>
#include <iostream>

ControllerAccessor::ControllerAccessor()
{
    m_loginController = new LoginController(this);
    std::unique_ptr<QLocalSocket> socket(new QLocalSocket(nullptr));
    socket->connectToServer("/var/tmp/turret-a4/control");
    if (socket->waitForConnected(6000))
    {
        std::cerr << "Cannot connect to atp actor\n";
    }

    m_actorClient = new ActorClient(socket.release(),nullptr);


    QObject::connect(m_actorClient, SIGNAL(on_login_success_msg(const QString&)),
                     m_loginController, SLOT(on_login_success(const QString&)));
}
ControllerAccessor::~ControllerAccessor()
{
    if(m_loginController != NULL)
    {
        delete m_loginController;
        m_loginController = NULL;
    }
    if(m_ActorClient != NULL )
    {
        delete m_ActorClient;
        m_ActorClient = NULL;
    }
}

LoginController* ControllerAccessor::getLoginController()
{
    return m_loginController;
}

ActorClient* ControllerAccessor::getActorClient()
{
    return m_actorClient;
}
