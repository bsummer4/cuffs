#pragma once 

#include<Connection.hpp>
#include<map>
#include<vector>

extern int CMDEBUG;

/**
 * ConnectionManager for recorder and replayer programs.
 */
class ConnectionManager{
    public:
        ConnectionManager(std::string hostname, unsigned int port);
        bool addConnection(int key);
        bool removeConnection(int key);
        virtual bool sendMessage(int key, char* msg, int msgl);
        bool connectionExists(int key);
        bool spawnPrinterConnection();
    public:
        std::map< int, Connection* > connection_map;
        std::string hostname;
        unsigned int port;
};

/** 
 * Special Connection Class I need to tell me who just connected.
 * only used for the RemoteConnectionManager Object
 */
class MonitorConnection : public Connection{
    public:
        MonitorConnection(const char *switchbox_hostname, const int switchbox_port);
        /// @TODO: WHY IS THIS NOT BEING CALLED!?
        virtual void handleAnnounceMessage(SBMessage *msg);
        void resetConnectionList();
        std::vector<int> getNewConnections();
    private:
        std::vector<int> new_connections;
};

class RemoteConnectionManager : public ConnectionManager{
    public:
        RemoteConnectionManager(std::string hostname, unsigned int port);
        ~RemoteConnectionManager();
        bool addRemoteConnection(int key, std::string hostname);
        virtual bool sendMessage(int key, char* msg, int msgl);

    protected:
        // Stuff for the remote clients.
        bool remoteClients;
        std::string privateSwitchboxAddress;
        int privateSwitchboxPort;
        std::string mycwd;
        // Key, Address
        std::map< int, int > key_to_remote_address;
        MonitorConnection * forwarderConnection;
};
