#ifndef ControllerAccessor_H
#define ControllerAccessor_H

#include <qt/state/ActorClient.hpp>
#include <qt/state/LoginController.hpp>

class ControllerAccessor
{

public:
    explicit ControllerAccessor();
    ~ControllerAccessor();

    LoginController* getLoginController();
    ActorClient* getActorClient();

private:
    LoginController* m_loginController;
    ActorClient* m_actorClient;

};

#endif
