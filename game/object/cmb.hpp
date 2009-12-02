#pragma once
#include <map>
#include <string>
#include <queue>
#include <sstream>
#include <stdexcept>
#include "Connection.hpp"
#include "Interpreter.hpp"
#include "Threaded.hpp"
#include "lib.h"
#include "misc.hpp"

namespace cmb {
  using namespace std;
  class Event;
  class Queue;
  typedef priority_queue<Event> pqueue;
  typedef map<int, pqueue> processes;

  class Timestamp {
  public:
    int time;
    int order;
    Timestamp(int time, int order) : time(time), order(order) {}
    std::string extractTimestamp(std::string event);
    friend bool operator<(const Timestamp &x, const Timestamp& y) {
      return (x.time == y.time) ? (x.order < y.order) : (x.time < y.time); }
    friend bool operator==(const Timestamp& x, const Timestamp& y) {
      return (x.time == y.time && x.order == y.order ); }
    friend bool operator>(const Timestamp& x, const Timestamp& y) {
      return !(x<y) && !(x==y); }
    friend bool operator>=(const Timestamp& x, const Timestamp& y) {
      return !(x<y); }
    friend bool operator<=(const Timestamp& x, const Timestamp& y) {
      return (x<y || x==y); }
    friend std::ostream& operator<<(std::ostream& out,
                                    const Timestamp& c) {
      out << c.time << "." << c.order;
      return out; }
    friend std::istream& operator>>(std::istream &in, Timestamp &obj) {
      char dot;
      if (!(in >> obj.time >> dot >> obj.order) || !(dot == '.'))
        throw runtime_error("Invalid Timestamp");
      return in; }};

  class TimestampAdder {
  public:
    int time;
    TimestampAdder(int time) : time(time) {}
    vector <string> operator()(vector <string> &messages) {
      vector <string> result;
      int remaining = messages.size();
      if (!remaining) {
        ostringstream o; o << Timestamp(time, 0);
        result.push_back(o.str()); }
      FOREACH (vector <string>, it, messages) {
        ostringstream o; o << Timestamp(time, --remaining) << " " << *it;
        result.push_back(o.str()); }
      time++;
      return result; }};

  /**
   * A class that contains the Timestamp as well as the string representing
   * an event.
   *
   * This class holds a "CMB" event, and can be compared with other
   * Event objects in order to function correctly in the priority
   * queue.
   */
  struct Event {
    Timestamp eventOccurs;
    string eventString;
    /// Parses the time out of the message.
    Event(string message) : eventOccurs(-1, 0), eventString(message) {
      istringstream i(message); i >> eventOccurs; }

    Event(Timestamp time, string event)
      : eventOccurs(time), eventString(event) {}

    /// Need to do this to use in priority queue.
    /// http://www.codeguru.com/cpp/tic/tic0229.shtml
    /// This is actually backwards so that it gets ordered in the way
    /// we like for the priority queue.
    friend bool operator<(const Event& x, const Event& y) {
      return (x.eventOccurs > y.eventOccurs);}};

  /// A priority queue class that manages placing events into
  /// appropriate queues as well as determining if the global time can
  /// be advanced.
  class Queue {
    processes cmbQueue;

  public:
    /// Place the given Event message onto the queue associated with
    /// the given ProcessId.
    void queueMessage(int processId, Event event) {
      cmbQueue[processId].push(event); }

    /// All events before @ref time
    pqueue getEvents(Timestamp time) {
      pqueue result;
      if (getLowestTime() < time) return result;
      FOREACH (processes, it, cmbQueue) {
        pqueue &current_q(it->second);
        while (!current_q.empty()) {
          Event e = current_q.top();
          if (time.time < e.eventOccurs.time) break;
          current_q.pop();
          result.push(e); }}
      return result; }

    /// -1 If no advancement can be made, otherwise lowest time we can
    /// advance to.
    ///
    /// We can advance to a time x if every process has a message that
    /// is at with time >= x.
    Timestamp getLowestTime() {
      Timestamp lowest(-1, 0);
      Timestamp lowestTimeHighestOrder(-1, 0);
      FOREACH (processes, it, cmbQueue) {
        pqueue &current_q(it->second);
        if (current_q.empty()) return Timestamp(-1, 0);
        Timestamp current = current_q.top().eventOccurs;
        if (current.time <= lowest.time || lowest.time == -1) {
          lowest = current;
          if (current.order > lowestTimeHighestOrder.order) {
            lowestTimeHighestOrder = current; }}}
      if (lowestTimeHighestOrder.time == lowest.time &&
          lowestTimeHighestOrder.order != 0)
        return Timestamp(-1, 0);
      return lowest; }};

  template <typename H>
  class Synchronizer {
    Queue cmb;
    H handler;
    boost::mutex timeLock;
    long current_time;
  public:
    unsigned int currentTime() {
      boost::mutex::scoped_lock l(timeLock);
      return current_time; }
    Synchronizer(H handler)
      : handler(handler), current_time(-1) {}
    void handleEvent(string event) {
      int process;
      Timestamp time(0, -1);
      string message;
      { istringstream i(event);
        char space;
        i >> process >> time;
        i.get(space);
        message = misc::slurp(i); }
      Event e(time, message);
      //      cerr << "(Event :time " << e.eventOccurs.time << " :ordering "
      //           << e.eventOccurs.order
      //           << " :message \"" << e.eventString << "\""
      //           << ")" << endl;
      cmb.queueMessage(process, Event(time, message));

      // Check to see if we can run any events If cmb.getLowestTime
      // gives something besides -1 then we can advance time along.
      LOOP {
        Timestamp t = cmb.getLowestTime();
        if (t.time == -1 || t.order != 0) return;

        pqueue pq = cmb.getEvents(t);
        while (!pq.empty()) {
          Event e = pq.top();
          pq.pop();
          handler.handleEvent(e.eventString); }
        boost::mutex::scoped_lock l(timeLock);
        current_time = t.time; }}};}
