/*
  # Dispatcher2

  Usage: dispatcher [interval in ms]
  where interval must be less than 1000

  Reads lines from stdin.  Every 'interval' ms, send all the lines
  that we have recieved with a interval count at the beginning.  If no
  lines were received in a given interval, then print a single, blank
  line for that interval.  Because of this, the timing of the input
  effects the output.  So trying to test this by ./distpatcher < file
  will not work.

  At the end, the number of 'interval's that have passed and the
  number of times we have printed lines is outputed to standard
  output.

  ## Example

      $ ./dispatcher 999
      hi
      0 hi
      there
      1 there
      ^D
      timing error: 2 vs 2
*/

#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <iostream>
#include <assert.h>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include "Threaded.h++"
#include "misc.h++"
#include "lib.h"
using namespace std;
using namespace misc;

/// Functor that dumps everything on the queue into the handler when
/// called.  This would be a handful of lines if c++ supported damn
/// closures.  We also add a call count to the front of each mesage
/// and pass only the count if there are no messages on the queue.
template <typename H>
class Dispatcher {
public:
  Dispatcher(Queue <string> &q, H handler) : q(q), count_(0), h(handler) {}
  void operator()() { handleQueue(); }
  unsigned long count() { return count_; }
private:
  void handleQueue() {
    vector <string> lines(q.popAll());
    if (lines.empty()) { ostringstream o; o << count();
                         h.handleEvent(o.str()); }
    FOREACH (vector<string>, it, lines) {
      ostringstream o; o << count() << " " << *it;
      h.handleEvent(o.str()); }
    count_++; }
  H h;
  unsigned long count_;
  Queue <string> &q; };

SignalThunk <Dispatcher<Printer &>& > *g_s = NULL; // set in main
void send (int signum) { g_s->signal(); }

int main (int argc, char **argv) {
  // Handle Args
  unsigned long interval = 5;
  if (argc > 1) interval = atoi(argv[1]);
  if (!interval) return 1;

  // Setup objects, signal handling
  (void) signal(SIGALRM, send);
  Timer t(interval);
  Queue <string> q;
  Printer p;
  Dispatcher <typeof(p)&> d(q, p);
  LineReader <typeof(q)&> r(cin, q);
  SignalThunk <typeof(d)&> s(d);

  // Run everything
  g_s = &s; // Global used by 'send' signal handler
  ualarm(interval*1000, interval*1000);
  r.start();
  s.start();
  r.join();

  cerr << "Performed " << d.count() << " operations.  " << endl;
  return 0; }
