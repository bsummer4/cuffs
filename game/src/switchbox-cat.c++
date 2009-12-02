#include <unistd.h>
#include <iostream>
#include <assert.h>
#include "switchbox.h++"
#include "misc.h++"
using namespace std;
using namespace misc;

int main (int num_args, char **args) {
  assert(num_args == 3 && "usage: switchbox-cat hostname port");
  string hostname (args[1]);
  int port = atoi(args[2]);

  Printer p;
  Translator <typeof(p)&> decoder(p, switchbox::decode);
  Filter <typeof(decoder)&> filter(decoder, switchbox::not_admin_junk);
  switchbox::Connection <typeof(filter)&> c(hostname, port, filter);
  Translator <typeof(c)&> broadcast_encoder(c, switchbox::broadcast_encode);
  LineReader <typeof(broadcast_encoder)&> lr(cin, broadcast_encoder);
  lr.start();
  c.start();
  lr.join();
  return 0; }
