#pragma once

#include <string>
#include <queue>
#include <iostream>
#include <sstream>
#include "Threaded.hpp"
extern "C" {
# include <sys/time.h>
# include <errno.h>
# include <assert.h>
# include <poll.h>
# include <pthread.h>
# include "switchbox_client.h"
}

namespace switchbox {
  using namespace std;

  /// We can't just pass it to the string constructor because it might
  /// see a '\0' in the message header and stop copying prematurly.
  string stringify(SBMessage *message) {
    string result;
    char *bytes = (char*) message;
    for (int ii = 0; ii != message->size; ii++)
      result.push_back(bytes[ii]);
    return result; }
  SBMessage *destringify(string event) { return (SBMessage *) event.c_str(); }
  string decode_w_address(string event) {
    SBMessage *m = destringify(event);
    ostringstream o;
    o << m->from << " " << m->data;
    return o.str(); }
  string decode(string event) { return string(destringify(event)->data); }
  bool is_admin_junk(string event) {
    int rt = destringify(event)->routing_type;
    return (rt == ANNOUNCE || rt == ADMIN || rt == TYPE_ERROR ||
            rt == INVALID_TARGET || rt == ADMIN_FAIL); }
  bool not_admin_junk(string event) { return !is_admin_junk(event); }
  string broadcast_encode(string event) {
    SBMessage *m = string_to_message(BROADCAST, -1, -1,
                                    const_cast<char *>(event.c_str()));
    string result(stringify(m));
    free(m);
    return result; }

  /// Handles a connection to a switchbox.  All 'Events' are
  /// SBMessages wrapped in a std::string.  We run in a thread and all
  /// messages that come over the socket are immediately send the
  /// 'handler' (We briefly examine some messages to determine our
  /// address, but no messages are hidden from the handler).  To send
  /// a message, give it to this.handleEvent().
  template <typename H>
  class Connection : public Threaded {
  public:
    const string switchbox_hostname;
    const int switchbox_port;

    virtual void stop() {
      close_connection(_connection);
      this->Threaded::stop(); }

    Connection(const string switchbox_hostname, const int switchbox_port,
               H handler)
      : switchbox_hostname(switchbox_hostname),
        switchbox_port(switchbox_port),
        _address(-1), _connection(-1), handler(handler), sendLock() {
      _connection = open_connection(switchbox_hostname.c_str(),
                                    switchbox_port);
      /// _address will be set when we recieve our first message.
      if (_connection == -1)
        throw runtime_error("Couldn't connect to switchbox.  "); }

    ~Connection() {
      if (valid_socket(_connection))
        close_connection(_connection); }

    void handleEvent(string event) {
      SBMessage *m = (SBMessage *) event.c_str();
      sendMessage(m); }
    void address() { return _address; }
    bool know_address() { return _address != -1; }

    /// Recieves all messages and passes them to the handler.  If they
    /// are announcements, then pass them to our announcehandler first.
    /// We stop when stop() is called or when we lose our connection to
    /// the switchbox.
    void run() {
      while (!stopRequested) {
        SBMessage * result = switchbox_receive(_connection);
        if (!result) break;
        if (result->routing_type == ANNOUNCE)
          handleAnnounceMessage(result);
        handler.handleEvent(stringify(result));
        free (result);
        run(); }
      close_connection(_connection); }

  protected:
    int _address;
    Socket _connection;
    H handler;
    boost::mutex sendLock;

    /// Whenever we get an announcement we extract our address from it
    void handleAnnounceMessage(SBMessage *msg) {
      if (!know_address() && msg->from == msg->to)
        _address = msg->from; }

    void sendMessage(SBMessage *message) {
      boost::mutex::scoped_lock l(sendLock);
      bool success = switchbox_send(_connection, message);
      if (!success) stop(); }};}
