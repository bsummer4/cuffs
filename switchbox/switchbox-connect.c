// This is like switchbox-cat except it prepends the
// switchbox-address of the user who sent it to every message.  It
// also prints messages of the form:
//
//     -1 lost_connection #
//     -1 new_connection #
//
// This needs to be linked with helper.o (from switchbox-client-framework.c)

#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include "switchbox_client.h"

bool admin_junk(SBMessage *m) {
  int t = m->routing_type;
  return !(t == BROADCAST || t == UNICAST || t == MULTICAST); };

SBMessage *make_message(char *line) {
  return string_to_message(BROADCAST, -1, -1, line); }

bool logout_message(SBMessage *msg) {
  return (msg->routing_type == ANNOUNCE &&
          announcement_code(msg) == LOST_CONNECTION); }

bool login_message(SBMessage *msg) {
  return (msg->routing_type == ANNOUNCE &&
          announcement_code(msg) == NEW_CONNECTION); }

void print_message(SBMessage *msg) {
  if (!admin_junk(msg))
    printf("%d %s\n", msg->from, msg->data);
  else if (logout_message(msg))
    printf("-1 lost_connection %d\n", msg->from);
  else if (login_message(msg))
    printf("-1 new_connection %d\n", msg->from);}
