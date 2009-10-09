#pragma once

#include <string>
#include <queue>

extern "C" {
#include "switchbox_client.h"
#include <poll.h>
#include <pthread.h>
}

/**
 * The connection class.
 *
 * The connection class allows a client to connect to a switchbox and
 * communicate through it. It provides methods, as well as
 * sending/receiving queues to abstract away communication from the
 * client.
 *
 * - TODO The message queue should be a separate class with it's own
 *   locking mechanism.
 */
class Connection {
public:
  Connection(const char* switchbox_hostname, const int switchbox_port);

  // Put a message on the send queue
  // This will free the message when it's finished.  So make a copy if
  // you want to keep it around.
  void sendMessage(SBMessage *message);

  /// Create a message to put on the send queue.
  void sendMessage(int size, message_type_t type, int to, char *string);

  /// Get a message off the receive queue.
  SBMessage * getMessage();

  /// Start the messaging thread.
  void start();

  /// Stop the messaging thread.
  void stop();

  /// Get the count of messages on the recieve queue.
  int getMessageCount();

  /// Get the address of this client.
  int getAddress();

  /// Check to see if the messaging thread is running
  bool isRunning();

  /// A convienence function that will block until there is a message
  /// on the messaging queue.
  void blockForMessage();

  /// A convienence function that will block until there is a message
  /// on the messaging queue or the given timeout expires.
  bool blockForMessage(int msec_timeout);

  /// Clear out the receive message queue
  void clearMessageQueue();

  /// * TODO Why aren't these private if you shouldn't need to call
  ///   them?

  /// Function for the receive messaing thread to call, you shouldn't
  /// need to call this.
  void receiveUpdate();

  /// Function for the send messaing thread to call, you shouldn't
  /// need to call this.
  void sendUpdate();

  virtual void handleAnnounceMessage(SBMessage * msg);

private:
  Socket connection;
  pthread_mutex_t receive_queue_lock;
  std::queue<SBMessage*> receive_queue;
  pthread_mutex_t send_queue_lock;
  pthread_cond_t message_ready_to_be_sent;
  std::queue<SBMessage*> send_queue;
  pthread_cond_t blocking_for_message;
  int address;
  int switchbox_port;
  bool messaging_threads_running;
  std::string switchbox_hostname;
  pthread_t receive_thread;
  pthread_t send_thread;
  struct pollfd descriptors_for_poll[1];
};
