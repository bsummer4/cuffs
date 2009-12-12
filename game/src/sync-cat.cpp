/*
@addtogroup sync_cat sync-cat
@ingroup Game

@section Sync_cat

@verbatim
usage: ./sync_cat [ids-to-synchronize-from]
@endverbatim

This listens to all addresses on the switchbox that actually send
stuff, synchronizes their outputs, and print the resulting messages.
This is very useful for testing the syncronizer, but also for doing
synchronization on programs that weren't designed for it.

Timestamp and sender information is expected on input an is not
printed on output.  Example usage:

@verbatim
  ./sync_cat 0 1
  0 0.0 hi
  1 0.0 there
  hi # output
  there # output
  0 0.1 hihihi
  ^D
@endverbatim
*/

#include <assert.h>
#include <unistd.h>
#include <vector>
#include <stdexcept>
#include "cmb.hpp"
#include "misc.hpp"
#include "Connection.hpp"
#include "Interpreter.hpp"
#include "lib.h"
#include "macro.h"

using namespace std;
using namespace misc;

int main (int argc, char **argv) {
  Printer p;
  vector <int> clients;
  FORII(argc) {
    if (!ii) continue;
    clients.push_back(atoi(argv[ii])); }

  cmb::Synchronizer <typeof(p)&> sync(p, clients);
  LineReader <typeof(sync)&> lr(cin, sync);
  lr.start();
  lr.join();
  return 0; }
