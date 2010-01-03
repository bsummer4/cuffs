// Simply prints to stdout all messages received from the switchbox,
// and broadcasts all messages from stdin.  Very useful for debugging,
// and tying various unixy programs together over a switchbox.
//
// This needs to be linked with helper.o (from switchbox-client-framework.c)

#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include "switchbox-client.h"

bool admin_junk(SBMessage *m) {
  int t = m->routing_type;
  return !(t == BROADCAST || t == UNICAST || t == MULTICAST); };

SBMessage *make_message(char *line) {
  return string_to_message(BROADCAST, -1, -1, line); }

void print_message (SBMessage *m) {
  if (!admin_junk(m))
    printf("%s\n", message_data(m)); }
