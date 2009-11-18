#include "ConnectionManager.hpp"
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>

#include <Connection.cpp>
extern "C"{
#include <switchbox_client.h>
}

using namespace std;

// Global mutex for writing to stderr
//boost::mutex stdout_lock;

/** 
 * PrinterConnection takes the Connection function and 
 * just prints any message it gets to stdout instead of queuing it. 
 */
class PrinterConnection : public Connection{
    public:
    //PrinterConnection(int _key, const char * switchbox_hostname, const int switchbox_port) 
    //    : Connection(switchbox_hostname, switchbox_port){this->key = _key;}
    PrinterConnection(const char * switchbox_hostname, const int switchbox_port) 
        : Connection(switchbox_hostname, switchbox_port){};
    

    virtual void receiveUpdate(){
        // Get message
        //cout << "Before Receive" << endl;
        SBMessage* result = switchbox_receive(this->connection);
        //cout << "After Receive" << endl;

        Connection::handleAnnounceMessage(result);

        // Lock writing mutex
        //boost::mutex::scoped_lock l(stdout_lock);

        // Special case for client joining.
        if ( result->routing_type == ANNOUNCE ){
            if ( result->from != result->to ){
                admin_message * adm = (admin_message*)result->data;
                if ( adm->task == NEW_CONNECTION ){
                    cout << "-1 new_connection " << result->from << endl;
                }
                else if ( adm->task == LOST_CONNECTION ){
                    cout << "-1 lost_connection " << result->from << endl;
                }
            }
        }
        else { 
            // Normal message, write to stdout
            cout << result->from << " " << result->data;
        }
        // Done
    }

    private:
      int key;
};

ConnectionManager::ConnectionManager(std::string hostname, unsigned int port, bool printer_mode){
    this->hostname = hostname;
    this->port = port;
    this->printer_mode = printer_mode;
}

/**
 * Assocate a new connection with the ConnectionManager
 *
 * @param key The key to associate with this connection.(or id)
 *
 * @return True if it added sucessfully, False if it didn't.
 * (Likely false implies that something with that key already exists.)
 */
bool ConnectionManager::addConnection(int key){
    std::map< int, Connection* >::iterator it;
    it = connection_map.find(key);

    if ( it != connection_map.end()){
        return false;
    }
    Connection * connection;
    if ( printer_mode ) 
        connection = new PrinterConnection(hostname.c_str(), port);
    else 
        connection = new Connection(hostname.c_str(), port);
    connection->start();
    connection_map.insert(std::pair< int, Connection* >(key, connection));
    return true;
}

/**
 * Remove (disconnect) a connection from the ConnectionManager
 * 
 * This function will cause the Connection to disconnect from the switchbox.
 * The memory will also have delete called on it.
 *
 * @param key The key to associate with this connection.(or id)
 * @param connection The pointer to the connection object. 
 *
 * @return True if it added sucessfully, False if it didn't.
 * (Likely false implies that something with that key already exists.)
 */
bool ConnectionManager::removeConnection(int key){
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
    return false;
}

/**
 * Send a message with the given key'd connection.
 *
 * @param key The key of the message to send. 
 * @param msg The message you wish to send.
 * @param msgl The length of the message. 
 * 
 * @returns True if the message sent, False if failed.
 */
bool ConnectionManager::sendMessage(int key, char* msg, int msgl){
    std::map< int, Connection* >::iterator it;
    it = connection_map.find(key);
    if ( it != connection_map.end()){
        cout << "sent Message: " << msg << endl;
        (*it).second->sendMessage(msgl+4*sizeof(int), BROADCAST, -1, msg);
        return true;
    }
    return false;
}

void ConnectionManager::printMessages(int key){
    std::map< int, Connection* >::iterator it;
    it = connection_map.find(key);
    if ( it != connection_map.end()){
        cout << (*it).second->getMessageCount() << endl;
    }
}
