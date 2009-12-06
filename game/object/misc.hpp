/// Reads lines from a stream and passes them to something that
/// implements a handleEvent method.
#pragma once
#include <queue>
#include <iostream>
#include <sstream>
#include <boost/function.hpp>
#include "Threaded.hpp"
#include "macro.h"
#include "lib.h"

namespace misc {
  using namespace std;

  string slurp(istream& ifs) {
    stringstream sstr;
    sstr << ifs.rdbuf();
    return sstr.str(); }

  string drop_first_word(string s) {
    istringstream i(s);
    string first;
    char space;
    i >> first >> space;
    return slurp(i); }

  /// This is a threadsafe queue implementation.  Things come off the
  /// queue in the same order they were added.
  template <typename T>
  class Queue {
  public:
    Queue() : q(), _lock() {}
    void handleEvent(T event) { boost::mutex::scoped_lock l(_lock);
                                q.push(event); }
    void pop() { boost::mutex::scoped_lock l(_lock);
                 q.pop(); }
    bool empty() { return q.empty(); }
    T front() {
      boost::mutex::scoped_lock l(_lock);
      T result(q.front());
      return result; }
    /// This only locks once, so everything happens atomically and
    /// there is less overhead.
    vector <T> popAll () {
      boost::mutex::scoped_lock l(_lock);
      vector <T> result;
      while (!q.empty()) {
        result.push_back(q.front());
        q.pop(); }
      return result; }
  private:
    queue <T> q;
    boost::mutex _lock; };

  /// Uses a predicate functor or function pointer to filter out unwanted
  /// messages.
  template <typename H>
  class Filter {
  public:
    typedef boost::function<bool (string)> P;
    Filter(H handler, P predicate)
      : handler(handler), predicate(predicate) {}
    void handleEvent(string e) { if (predicate(e)) handler.handleEvent(e); }
  private:
    H handler;
    P predicate; };

  /// Uses a functor or function pointer to translate between two
  /// message formats.
  template <typename H>
  class Translator {
  public:
    typedef boost::function<string (string)> T;
    Translator(H handler, T translate)
      : handler(handler), translate(translate) {}
    void handleEvent(string e) { handler.handleEvent(translate(e)); }
  private:
    H handler;
    T translate; };

  /// Reads lines from a stream and sends them to a signal handler.
  /// Max line length is 1024
  template <typename H>
  class LineReader : public Threaded {
    istream &in;
    H h;
  public:
    LineReader(istream &in, H handler) : in(in), h(handler) {}
    void run() {
      char buffer[1024];
      while (!stopRequested && in.getline(buffer, 1024))
        h.handleEvent(string(buffer)); }};

  /// Basically lets you attach cout to an event handler.
  class Printer {
  public:
    void handleEvent(string s) { cout << s << endl; }};

  /// Pass everything in the vector to an event handler
  template <typename T>
  void handleAll(vector <string> &data, T handler) {
    FOREACH(vector <string>, it, data)
      handler.handleEvent(*it); }

  /// Pass everything in the queue to an event handler
  template <typename T>
  void handleAll(Queue <string> & q, T handler) {
    vector <string> data(q.popAll());
    handleAll <T> (data, handler); }

  /// Calls a thunk every time 'go' is signaled.  We maintain our own
  /// thread and the thunk is run in it.
  template <typename T>
  class SignalThunk : public Threaded {
  public:
    SignalThunk(T thunk) : thunk(thunk) {
      pthread_mutex_init(&lock, NULL);
      pthread_cond_init(&go, NULL); };
    void run() { if (stopRequested) return;
                 wait_for_signal();
                 thunk();
                 run(); }
    void signal() { pthread_cond_signal(&go); }
  private:
    void wait_for_signal() {
      pthread_mutex_lock(&lock);
      pthread_cond_wait(&go, &lock);
      pthread_mutex_unlock(&lock); }
    pthread_cond_t go;
    pthread_mutex_t lock;
    T thunk; };

  class Timer {
  public:
    Timer(unsigned long interval) : interval(interval) {
      gettimeofday(&start, &ignore_this); }
    ~Timer() {
      gettimeofday(&stop, &ignore_this);
      unsigned long intervals = (timeval_diff(&start, &stop) /
                                 (1000 * interval));
      cerr << "Ran for " << intervals << " intervals.  " << endl; }
  private:
    unsigned long interval;
    struct timeval start, stop;
    struct timezone ignore_this;
  };}
