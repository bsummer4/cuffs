#pragma once 

#include<Connection.hpp>
#include<map>

/**
 * ConnectionManager for recorder and replayer programs.
 */
class ConnectionManager{
    public:
        ConnectionManager(std::string hostname, unsigned int port, bool printer_mode=false);
        bool addConnection(int key);
        bool removeConnection(int key);
        bool sendMessage(int key, char* msg, int msgl);
    public:
        std::map< int, Connection* > connection_map;
        std::string hostname;
        unsigned int port;
        bool printer_mode;
};
