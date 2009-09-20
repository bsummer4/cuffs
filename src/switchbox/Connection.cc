#include "Connection.h"

// Prototype for thread.
extern "C" void * messaging_thread(void* arg);

/**
 * The constructor for the object. 
 *
 * @param hostname The hostname of the switchbox.
 * @param port The port that the switchbox is running on.
 */
Connection::Connection(const char* hostname, const int port){
    this->s = -1;
    this->hostname = hostname;
    this->from = -1;
    this->port = port;
    running = false;

    pthread_mutex_init(&rlock, NULL);
    pthread_mutex_init(&slock, NULL);
    pthread_cond_init(&blocker, NULL);
}

void Connection::start(){
    this->s = open_connection(hostname.c_str(), port);
    fds_[0].fd = this->s;
    fds_[0].events = POLLIN;
    this->running = true;
    pthread_create(&tid, NULL, messaging_thread, reinterpret_cast<void*>(this));
}

void Connection::stop(){
    this->s = open_connection(hostname.c_str(), port);
    this->running = false;
    pthread_join(tid, NULL);
}

/**
 * Function to send a message. This function will actually appened the message to a queue
 * and the messaging thread will send the message. 
 *
 * @param size The size of the message, including the header, which is 4 ints. 
 * @param type The type of message. 
 * @param to The address of the user this message is for. 
 * @param string The message. 
 */
void Connection::sendMessage(int size, message_type type, int to, char *string){
    SBMessage *result = (SBMessage*)malloc(size);
    if ( !result ) {
        perror("malloc");
        exit(1);
    }
    //result->size = size + sizeof(int)*4;
    result->size = size;
    result->routing_type = type;
    result->from = this->from;
    result->to = to;
    memcpy(&result->data, string, size - 4 * sizeof(int));
    pthread_mutex_lock(&slock);
    send_queue.push(result);
    pthread_mutex_unlock(&slock);
}

/**
 * @return The number of messages in the receive queue.
 */
int Connection::getMessageCount(){
    return receive_queue.size();
}

/**
 * @return The address of this client
 */
int Connection::getAddress(){
    return this->from;
}

/** 
 * @return The first message in the messaging queue.
 */
SBMessage * Connection::getMessage(){
    if ( receive_queue.empty() ){
        return NULL;
    } else { 
        pthread_mutex_lock(&rlock);
        SBMessage* ret = receive_queue.front();
        receive_queue.pop();
        pthread_mutex_unlock(&rlock);
        return ret;
    }
}

void Connection::handleAnnounceMessage(SBMessage * msg){
    if (msg){
        if ( msg->from == msg->to ){
            // If this is the case, then this is my address
            this->from = msg->from;
        }
        free(msg);
    }
}

/** 
 * The message updating thread. Started with start()
 * and stopped with stop(). Handles all of the actual
 * sending/receiving of the messages.
 */ 
void Connection::messageUpdate(){
    static const int timeout_msecs = 10;
    
    int numbytes = poll(fds_, 1, timeout_msecs);

    if ( numbytes > 0 ){
        SBMessage * result = switchbox_receive(this->s);

        if ( result->routing_type == ADMIN ){
            handleAnnounceMessage(result);
        }
        else{ 
            // Append to our receive queue
            pthread_mutex_lock(&rlock);
            receive_queue.push(result);
            pthread_mutex_unlock(&rlock);
            pthread_cond_signal(&blocker);
        }
    }

    // Check if there's any messages to send
    while ( !send_queue.empty() ){
        pthread_mutex_lock(&slock);
        SBMessage* msg = send_queue.front();
        send_queue.pop();
        pthread_mutex_unlock(&slock);
        switchbox_send(this->s, msg);
    }
}


/**
 * This function will return when there
 * is a message on the receive queue.
 */
void Connection::blockForMessage(){
    if ( receive_queue.empty() ){
        pthread_mutex_lock(&rlock);
        pthread_cond_wait(&blocker, &rlock);
        pthread_mutex_unlock(&rlock);
    }
}

void Connection::clearMessageQueue(){
    pthread_mutex_lock(&rlock);
    while ( !receive_queue.empty() )
        receive_queue.pop();
    pthread_mutex_unlock(&rlock);
}

/**
 * @returns True if the messaging thread is running, otherwise
 * returns false.
 */
bool Connection::isRunning(){
    return running;
}


extern "C" void * messaging_thread(void* arg){
    Connection * c = reinterpret_cast<Connection*>(arg);
    while(c->isRunning()){
        c->messageUpdate();
        //pthread_yield();
    }
    pthread_exit(NULL);
}
