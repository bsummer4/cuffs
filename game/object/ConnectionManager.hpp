#pragma once

#include <map>
#include <vector>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>

#include <Connection.cpp>
extern "C"{
#include <switchbox_client.h>
}

namespace sb_io{
bool CMDEBUG=false;
using namespace std;

/**
 * PrinterConnection takes the Connection function and
 * just prints any message it gets to stdout instead of queuing it.
 */
class PrinterConnection : public Connection{
    public:
    PrinterConnection(const char * switchbox_hostname, const int switchbox_port)
        : Connection(switchbox_hostname, switchbox_port){};


    virtual void
    receiveUpdate(){
        // Get message
        SBMessage* result = switchbox_receive(this->connection);
        this->handleAnnounceMessage(result);
        // Special case for client joining.
        if ( result->routing_type == ANNOUNCE ){
            if ( result->from != result->to ){
                admin_message * adm = (admin_message*)result->data;
                if ( adm->task == NEW_CONNECTION ){
                    cout << "-1 new_connection " << result->from << endl;}
                else if ( adm->task == LOST_CONNECTION ){
                    cout << "-1 lost_connection " << result->from << endl;}}}
        else {
            // Normal message, write to stdout
            cout << result->from << " " << result->data;}}
    private:
      int key;};

/**
 * ConnectionManager for recorder and replayer programs.
 */
class ConnectionManager{
    public:
        ConnectionManager(std::string hostname, unsigned int port)
            : hostname(hostname), port(port){
                assert(spawnPrinterConnection());}
        /**
         * Assocate a new connection with the ConnectionManager
         *
         * @param key The key to associate with this connection.(or id)
         *
         * @return True if it added sucessfully, False if it didn't.
         * (Likely false implies that something with that key already exists.)
         */
        bool addConnection(int key){
            std::map< int, Connection* >::iterator it;
            it = connection_map.find(key);

            if ( it != connection_map.end()){
                return false;
            }
            Connection * connection;
            connection = new Connection(hostname.c_str(), port);
            connection->start();
            assert(connection->isRunning());
            connection_map.insert(std::pair< int, Connection* >(key, connection));
            return true;}
        /**
         * Remove (disconnect) a connection from the ConnectionManager
         *
         * This function will cause the Connection to disconnect from the switchbox.
         * The memory will also have delete called on it.
         *
         * @param key The key to associate with this connection.(or id)
         *
         * @return True if it added sucessfully, False if it didn't.
         * (Likely false implies that something with that key already exists.)
         */
        bool removeConnection(int key){
            std::map< int, Connection* >::iterator it;
            it = connection_map.find(key);
            if ( it != connection_map.end()){
                Connection* con = (*it).second;
                cout << (*it).second->getMessageCount() << endl;
                con->stop();
                connection_map.erase(it);
                delete con;
                return true;
            }
            return false;}
        /**
         * Send a message with the given key'd connection.
         *
         * @param key The key of the message to send.
         * @param msg The message you wish to send.
         * @param msgl The length of the message.
         *
         * @returns True if the message sent, False if failed.
         */
        virtual bool sendMessage(int key, char* msg, int msgl){
            std::map< int, Connection* >::iterator it;
            it = connection_map.find(key);
            if ( it != connection_map.end()){
                if (CMDEBUG) cerr << "ConnectionManager::sendMessage():"
                    << " sent Message: " << msg << endl;
                (*it).second->sendMessage(msgl+4*sizeof(int), BROADCAST, -1, msg);
                return true;
            }
            return false;}

        bool connectionExists(int key){
            std::map< int, Connection* >::iterator it;
            it = connection_map.find(key);
            return ( it != connection_map.end() );}

        bool spawnPrinterConnection(){
            int key = 0;
            std::map< int, Connection* >::iterator it;
            it = connection_map.find(key);
            if ( it != connection_map.end()){
                return false;
            }
            Connection * connection;
            connection = new PrinterConnection(hostname.c_str(), port);
            connection->start();
            assert(connection->isRunning());
            usleep(10000);
            assert(connection->getAddress() >= 0);
            if ( connection->getAddress() != 0 ){
                cerr << "Warning: The Printer connection's address is not 0. This"        << endl
                     << "         likely means that the input will not match the output!" << endl;
            }
            connection_map.insert(std::pair< int, Connection* >(key, connection));
            if (CMDEBUG) cerr << "ConnectionManager::spawnPrinterConnection(): success" << endl;
            return true;}

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
        MonitorConnection(const char *switchbox_hostname, const int switchbox_port)
            : Connection(switchbox_hostname,switchbox_port) {};
        virtual void handleAnnounceMessage(SBMessage *msg){
            if (CMDEBUG) cerr << "MonitorConnection::handleAnnounceMessage()" << endl;
            if ( msg->from != msg->to ){
                admin_message * adm = (admin_message*)msg->data;
                if ( adm->task == NEW_CONNECTION ){
                    new_connections.push_back(msg->from);}
            }
            // Now that we're done inspecting the message let the connection object do
            // its real work.
            return Connection::handleAnnounceMessage(msg);}
        void resetConnectionList(){
            new_connections.clear();}
        std::vector<int> getNewConnections(){
            return new_connections;}
    private:
        std::vector<int> new_connections;
};

class RemoteConnectionManager : public ConnectionManager{
    public:
        RemoteConnectionManager(std::string hostname, unsigned int port)
            : ConnectionManager(hostname,port), remoteClients(false),
            privateSwitchboxPort(3214), forwarderConnection(NULL){}

        ~RemoteConnectionManager(){
            char buf[512];
            if (CMDEBUG) cerr << "Destructor" << endl;
            if ( remoteClients ){
                if (CMDEBUG) cerr << "Closing all remote connections" << endl;
                // Send a close message for everyone and then shut off the switchbox.
                std::map<int,int>::iterator it;
                for (it = key_to_remote_address.begin(); it != key_to_remote_address.end(); it++){
                    snprintf(buf,512, "lost_connection %i\n" , it->second);
                    sendMessage(it->first, buf, strlen(buf));}
                /// @TODO Just wait and hope thats enough for now.
                usleep(100000);
                if (CMDEBUG) cerr << "Turning off the private switcbhox" << endl;
                system("./switchbox.sh stop");}}
        bool addRemoteConnection(int key, std::string remote_hostname){
            char command[2048];
            if ( !remoteClients ){
                // I am the first, so I need to spawn a switchbox and create a connection to it.
                system("./switchbox.sh start");
                char buf[1024];
                if ( 0 != gethostname(buf,1024)){
                    perror("gethostname: ");
                    exit(1);}
                privateSwitchboxAddress = buf;
                if ( NULL == getcwd(buf,1024) ){
                    perror("getcwd(): ");
                    exit(1);}
                mycwd = buf;
                forwarderConnection = new MonitorConnection(
                                                  privateSwitchboxAddress.c_str(),
                                                  privateSwitchboxPort);
                remoteClients = true;
                usleep(100000);}
            forwarderConnection->resetConnectionList();
            snprintf(command, 2048, "ssh -f %s \"%s/repeater %s %i %s %i %i\"",
                    remote_hostname.c_str(),
                    mycwd.c_str(),
                    hostname.c_str(),
                    port,
                    privateSwitchboxAddress.c_str(),
                    privateSwitchboxPort,
                    key);
            if (CMDEBUG) cerr << command << endl;
            assert(0 == system(command));
            vector<int> newConnections;
            int counter = 0;
            while ( newConnections.size() == 0 ){
                /// @TODO This is kind of ugly just polling, if it turns out to
                /// become wasteful make sure to change it.
                usleep(10000);
                newConnections = forwarderConnection->getNewConnections();
                counter++;
                assert(counter<10000);}
            assert(newConnections.size()==1);
            key_to_remote_address.insert( std::pair<int,int>(key, newConnections[0]));
            return true;}
        virtual bool sendMessage(int key, char* msg, int msgl){
            std::map<int,int>::iterator it;
            it = key_to_remote_address.find(key);
            if ( it != key_to_remote_address.end() ){
                forwarderConnection->sendMessage(msgl+4*sizeof(int),
                                                 UNICAST,
                                                 it->second,
                                                 msg);
                return true;}
            else{
                if(CMDEBUG) cerr << "Couldn't send remotely sending using "
                                 << "ConnectionManager::sendMessage()" << endl;
                return ConnectionManager::sendMessage(key,msg,msgl);}}

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


};
