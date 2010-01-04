/**
@defgroup delayed_cat delayed-cat
@ingroup Game
@section Description
This just reads all lines from stdin, and then, after standard
input is closed, prints them all out.  This is a test-utillity for
Queue and LineReader
@sa cat
**/

#include <unistd.h>
#include <iostream>
#include <assert.h>
#include "misc.hpp"
using namespace std;
using namespace misc;

int main (int num_args, char **args) {
  assert(num_args == 1 && "We don't accept any arguments.  ");
  Queue <string> q;
  misc::Printer p;
  LineReader <Queue <string>& > r(cin, q);
  r.start();
  r.join();
  misc::handleAll <Printer&> (q, p);
  return 0; }
