// Simply prints to stdout all messages received from the switchbox,
// and broadcasts all messages from stdin.  Very useful for debugging,
// and tying various unixy programs together over a switchbox.

#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include "switchbox_client.h"

// Returns global memory.  Don't hold on to it, just use it then forget about
// it. This only accepts lines with length <= 256, otherwise we EXIT!
char *get_line() {
  static char buffer[256];
  char *result = fgets(buffer, 256, stdin);
  // otherwise strlen(NULL) will segfault
  if (!result) return NULL;
  // Make sure the line wan't too long
  assert("What the fuck to you thing you're doing?  That line is too long!" && \
         ((strlen(result) < 255) || (result[255] == '\n')));
  return result; }

bool admin_junk(SBMessage *m) {
  int t = m->routing_type;
  return !(t == BROADCAST || t == UNICAST || t == MULTICAST); };

void *stdin_to_switchbox_broadcast(void *socket_v) {
  int socket = * (int*) socket_v;
  char *line;
  while ((line = get_line())) {
    SBMessage *m = string_to_message(BROADCAST, -1, -1, line);
    if (!switchbox_send(socket, m)) { free(m); break; }
    free(m); }
  fflush(stdout);
  pthread_exit(NULL); }

void *switchbox_to_stdout(void *socket_v) {
  int socket = * (int*) socket_v;
  SBMessage *m;
  while ((m = switchbox_receive(socket)))
    if (!admin_junk(m))
      printf("%s", message_data(m)); 
  pthread_exit(NULL); }

int main (int num_args, char **args) {
  assert("usage: switchbox-cat hostname port" && num_args == 3);
  const char *hostname = args[1];
  const int port = atoi(args[2]);
  int socket = open_connection(hostname, port);
  assert("Couldn't connect to switchbox" && valid_socket(socket));

  pthread_t in, out;
  pthread_create(&out, NULL, stdin_to_switchbox_broadcast, &socket);
  pthread_create(&in, NULL, switchbox_to_stdout, &socket);
  pthread_join(out, NULL);
  close_connection(socket);
  pthread_join(in, NULL);
  return 0; }
