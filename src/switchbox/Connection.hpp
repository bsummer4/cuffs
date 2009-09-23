#pragma once

#include <string>
#include <queue>

extern "C"
{
#include "switchbox_client.h"
#include <poll.h>
#include <pthread.h>
}

/** @defgroup Switchbox Switchbox
 * The switchbox and everything related.
 */
/** @defgroup ClientLib ClientLib
 * @ingroup Switchbox
 * C++ Client library for communication via switchbox.
 */

/** @addtogroup ClientLib
 * @{
 */

/**
 * The connection class.
 *
 * The connection class allows a client to connect to a switchbox and communicate through it. It
 * provides methods, as well as sending/receiving queues to abstract away communication from the
 * client.
 */
class Connection
{
public:
  /// Constructor
  Connection (const char* hostname, const int port);
  /// Create a message to put on the send queue.
  void sendMessage (int size, message_type type, int to, char *string);
  /// Get a message off the receive queue.
  SBMessage * getMessage();
  /// Start the messaging thread.
  void start();
  /// Stop the messaging thread.
  void stop();
  /// Function for the receive messaing thread to call, you shouldn't need to call this.
  void receiveUpdate();
  /// Function for the send messaing thread to call, you shouldn't need to call this.
  void sendUpdate();
  /// Get the count of messages on the recieve queue.
  int getMessageCount();
  /// Get the address of this client.
  int getAddress();
  /// Check to see if the messaging thread is running
  bool isRunning();
  /// A convienence function that will block until there is a message on the messaging queue.
  void blockForMessage();
  /// Clear out the receive message queue
  void clearMessageQueue();
private:
  /// Internal function to handle special messages.
  void handleAnnounceMessage (SBMessage * msg);
private:
  /// The socket.
  Socket s;
  /// A lock to access the receive queue.
  pthread_mutex_t rlock;
  /// The receive queue.
  std::queue<SBMessage*> receive_queue;
  /// A lock to access the send queue.
  pthread_mutex_t slock;
  /// Condition Variable to notify that there's a message to send.
  pthread_cond_t scond;
  /// The send queue.
  std::queue<SBMessage*> send_queue;
  /// A conditiona variable used by the blockForMessage() function.
  pthread_cond_t blocker;
  /// The address of this client
  int from;
  /// The port of the switchbox that I'm connected to.
  int port;
  /// Flag for if the messaing thread is running.
  bool running;
  /// The hostname of  the switchbox.
  std::string hostname;
  /// A thread ID for the receive messaging thread.
  pthread_t r_tid;
  /// A thread ID for the send messaging thread.
  pthread_t s_tid;
  /// File descriptors for the poll command.
  struct pollfd fds_[1];
};

/**
 * @}
 */
