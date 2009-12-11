/*
  # Sync_cat

  usage: ./sync_cat server port

  This listens to all addresses on the switchbox that actually send
  stuff, synchronizes their outputs, and print the resulting messages.
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
