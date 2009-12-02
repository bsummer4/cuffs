/// Simple implementation of the unix utillity 'cat' using the event
/// handler architecture.
#include <assert.h>
#include "misc.h++"
using namespace misc;

/// Intercept 'sleep time' and 'usleep time' commands and cause the
/// caller of handleEvent to wait accoringly
template <typename H>
class Sleeper {
  H handler;
public:
  Sleeper(H handler) : handler(handler) {};
  void handleEvent(string event) {
    istringstream i(event);
    string command;
    int time;
    i >> command >> time;
    if (!command.compare("usleep")) { usleep(time);
                                      return; }
    if (!command.compare("sleep")) { sleep(time);
                                     return; }
    handler.handleEvent(event); }};

int main (int argc, char **argv) {
  Printer p;
  Sleeper <typeof(p)&> s(p);
  LineReader <typeof(s)&> r(cin, s);
  r.start();
  r.join(); }
