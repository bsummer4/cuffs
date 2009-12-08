#include <unistd.h>
#include <iostream>
#include <assert.h>
#include "switchbox.hpp"
#include "misc.hpp"
using namespace std;
using namespace misc;
using namespace switchbox;

bool empty(string s) { return !s.size(); }
bool notempty(string s) { return !empty(s); }

bool logout_message(SBMessage *msg) {
  return (msg->routing_type == ANNOUNCE &&
          announcement_code(msg) == LOST_CONNECTION); }

string translate(string s) {
  if (not_admin_junk(s))
    return decode_w_address(s);
  SBMessage *msg = destringify(s);
  if (logout_message(msg)) {
    ostringstream o;
    o << "-1 lost_connection " << msg->from;
    return o.str(); }
  return ""; }

int main (int num_args, char **args) {
  assert(num_args == 3 && "usage: switchbox-cat hostname port");
  string hostname (args[1]);
  int port = atoi(args[2]);

  Printer p;
  Filter <typeof(p)&> filter(p, notempty);
  Translator <typeof(filter)&> decoder(filter, translate);
  Connection <typeof(decoder)&> c(hostname, port, decoder);
  Translator <typeof(c)&> broadcast_encoder(c, broadcast_encode);
  LineReader <typeof(broadcast_encoder)&> lr(cin, broadcast_encoder);
  lr.start();
  c.start();
  lr.join();
  return 0; }
