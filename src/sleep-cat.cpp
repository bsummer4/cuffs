/**
@addtogroup sleep_cat sleep-cat
@ingroup Game

@section Description

Just like normal cat except when it sees a line that looks like
one of:
   sleep #
   usleep #
It sleeps for that ammount of time and then doesn't print that
line

This is useful for testing time-sensitive code.
**/

#include <assert.h>
#include "misc.hpp"
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
