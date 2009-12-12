/**
@addtogroup cat
@ingroup Game 
@section Description
Simple implementation of the unix utillity 'cat' using the event
handler architecture.
@sa delayed_cat
**/
#include <assert.h>
#include "misc.hpp"
using namespace misc;

int main (int argc, char **argv) {
  Printer p;
  LineReader <Printer> r(cin, p);
  r.start();
  r.join(); }
