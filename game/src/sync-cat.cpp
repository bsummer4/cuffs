/*
  # Sync_cat

  usage: ./sync_cat server port

  This listens to all addresses on the switchbox that actually send
  stuff, synchronizes their outputs, and print the resulting messages.
*/

#include <assert.h>
#include <unistd.h>
#include <stdexcept>
#include "cmb.h++"
#include "misc.h++"
#include "Connection.hpp"
#include "Interpreter.h++"
#include "lib.h"

using namespace std;
using namespace misc;

int main (int argc, char **argv) {
  Printer p;
  cmb::Synchronizer <typeof(p)&> sync(p);
  LineReader <typeof(sync)&> lr(cin, sync);
  lr.start();
  lr.join();
  return 0; }
