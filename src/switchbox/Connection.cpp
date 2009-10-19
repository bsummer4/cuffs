#include "Connection.hpp"
extern "C"{
#include <sys/time.h>
#include <errno.h>
}

// Prototype for thread.
extern "C" void * receive_messaging_thread(void* arg);
extern "C" void * send_messaging_thread(void* arg);

/**
 * The constructor for the object.
 *
 * @param switchbox_hostname The hostname of the switchbox.
 * @param switchbox_port The port that the switchbox is running on.
 */
Connection::Connection(const char* switchbox_hostname,
                       const int switchbox_port) {
  this->connection = -1;
  this->switchbox_hostname = switchbox_hostname;
  this->address = -1;
  this->switchbox_port = switchbox_port;
  messaging_threads_running = false;

  pthread_mutex_init(&receive_queue_lock, NULL);
  pthread_mutex_init(&send_queue_lock, NULL);
  pthread_cond_init(&blocking_for_message, NULL);
  pthread_cond_init(&message_ready_to_be_sent, NULL);
}

void Connection::start() {
  this->connection = open_connection(switchbox_hostname.c_str(),
                                     switchbox_port);
  descriptors_for_poll[0].fd = this->connection;
  descriptors_for_poll[0].events = POLLIN;
  this->messaging_threads_running = true;
  pthread_create(&receive_thread, NULL, receive_messaging_thread,
                 reinterpret_cast<void*>(this));
  pthread_create(&send_thread, NULL, send_messaging_thread,
                 reinterpret_cast<void*>(this));
}

void Connection::stop() {
  this->messaging_threads_running = false;
  pthread_cancel(receive_thread);
  pthread_cancel(send_thread);
  pthread_join(receive_thread, NULL);
  pthread_join(send_thread, NULL);
}

void Connection::sendMessage(SBMessage *message) {
  pthread_mutex_lock(&send_queue_lock);
  send_queue.push(message);
  pthread_mutex_unlock(&send_queue_lock);
  pthread_cond_signal(&message_ready_to_be_sent);
}

/**
 * Function to send a message. This function will actually appened the
 * message to a queue and the messaging thread will send the message.
 *
 * @param size The size of the message, including the header, which is
 *        4 ints.
 * @param type The type of message.
 * @param to The address of the user this message is for.
 * @param string The message.
 */
void Connection::sendMessage(int size, message_type_t type, int to,
                             char *string) {
  SBMessage *result = message(size, this->address, to, type, string);
  if (!result) {
    perror("malloc");
    exit(1); /// @TODO Use an exception?
  }

  sendMessage(result);
}

/**
 * @return The number of messages in the receive queue.
 */
int Connection::getMessageCount() {
  return receive_queue.size();
}

/**
 * @return The address of this client
 */
int Connection::getAddress() {
  return this->address;
}

/**
 * @return The first message in the messaging queue.
 */
SBMessage * Connection::getMessage() {

  /// @TODO optional blocking here?
  if (receive_queue.empty()) return NULL;

  pthread_mutex_lock(&receive_queue_lock);
  SBMessage* ret = receive_queue.front();
  receive_queue.pop();
  pthread_mutex_unlock(&receive_queue_lock);
  return ret;
}

void Connection::handleAnnounceMessage(SBMessage * msg) {
  if (!msg) return;
  // If this is the case, then this is my address
  if (msg->from == msg->to)
    this->address = msg->from;
}

/**
 * The receive message updating thread. Started with start() and
 * stopped with stop(). Handles all of the actual sending/receiving of
 * the messages.
 */
void Connection::receiveUpdate() {
  static const int timeout_msecs = -1;

  int numbytes = poll(descriptors_for_poll, 1, timeout_msecs);

  if (numbytes > 0) {
    SBMessage * result = switchbox_receive(this->connection);

    if (result->routing_type == ANNOUNCE) {
      this->handleAnnounceMessage(result);
    } else {
      // Append to our receive queue
      pthread_mutex_lock(&receive_queue_lock);
      receive_queue.push(result);
      pthread_mutex_unlock(&receive_queue_lock);
      pthread_cond_signal(&blocking_for_message);
    }
  } else {
    pthread_exit(NULL);
  }
}

/**
 * The receive message updating thread. Started with start() and
 * stopped with stop(). Handles all of the actual sending/receiving of
 * the messages.
 */
void Connection::sendUpdate() {
  pthread_mutex_lock(&send_queue_lock);
  pthread_cond_wait(&message_ready_to_be_sent, &send_queue_lock);

  // Check if there's any messages to send
  while (!send_queue.empty()) {
    SBMessage* msg = send_queue.front();
    send_queue.pop();
    switchbox_send(this->connection, msg);
    free(msg);
  }
  pthread_mutex_unlock(&send_queue_lock);
}


/**
 * This function will return when there is a message on the receive
 * queue.
 */
void Connection::blockForMessage() {
  if (!receive_queue.empty()) return;
  pthread_mutex_lock(&receive_queue_lock);
  pthread_cond_wait(&blocking_for_message, &receive_queue_lock);
  pthread_mutex_unlock(&receive_queue_lock);
}

/**
 * This function will return when there is a message on the receive queue,
 * or if the timeout has been reached.
 */
bool Connection::blockForMessage(int msec_timeout)
{
  if (!receive_queue.empty()) return true;
  struct timeval tv;
  struct timespec ts;
  gettimeofday(&tv, NULL);
  ts.tv_sec = tv.tv_sec + msec_timeout/1000;
  ts.tv_nsec = tv.tv_usec * 1000 + (msec_timeout)%1000 * 1000000;
  //ts.tv_sec = ts.tv_nsec/(int)1e9;
  //ts.tv_nsec = ts.tv_nsec%(int)1e9;
  pthread_mutex_lock(&receive_queue_lock);
  int rc = pthread_cond_timedwait(&blocking_for_message, &receive_queue_lock,
                                  &ts);
  pthread_mutex_unlock(&receive_queue_lock);
  if (rc == 0) return true;
  if (rc == ETIMEDOUT) return false;

  perror("cond_timewait:");
  return false;
}

void Connection::clearMessageQueue() {
  pthread_mutex_lock(&receive_queue_lock);
  while (!receive_queue.empty())
    receive_queue.pop();
  pthread_mutex_unlock(&receive_queue_lock);
}

/**
 * @returns True if the messaging thread is running, otherwise returns
 * false.
 */
bool Connection::isRunning() {
  return messaging_threads_running;
}

extern "C" void * receive_messaging_thread(void* arg) {
  Connection * c = reinterpret_cast<Connection*>(arg);
  while (c->isRunning()) c->receiveUpdate();
  pthread_exit(NULL);
}

extern "C" void * send_messaging_thread(void* arg) {
  Connection * c = reinterpret_cast<Connection*>(arg);
  while (c->isRunning()) c->sendUpdate();
  pthread_exit(NULL);
}
