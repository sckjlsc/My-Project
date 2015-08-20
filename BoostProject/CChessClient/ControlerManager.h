#ifndef CONTROLERMANAGER_H
#define CONTROLERMANAGER_H

class NetworkManager;

#define  atpControlers (ControlerManager::instance())

class ControlerManager
{
public:
    ControlerManager(int &argc, char **argv);
    static ControlerManager* instance() { return self_; }

	bool init();
	bool connectSigSlot();

public:
    // run the service
    bool runProgram();
    // cleanup and exit
    bool exitProgram();


public: // cmd-line options
    bool isReactivating() const;
    bool exiting() const {return exiting_;}

public: // getter & setter
    NetworkManager* networkManager() const {return m_networkManager;}


protected:
	NetworkManager* m_networkManager;

    
private:
    static ControlerManager* self_;
    bool exiting_;
};
#endif