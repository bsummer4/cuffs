// This file is used when building switchbox-cat and
// switchbox-connect.  Compiling this file {gcc -c} produces an o file
// that is linked agains to produce those executables
//
// This is the basic structure of a program that gets message
// specifications as lines from stdin and sends them to the switchbox.
// Messages from the switchbox are send to stdout.  A couple of
// functions are left undefined, so that this can be done in various
// ways.  To use this, define functions for all the prototypes
// directly below the includes, then link against this files resulting
// .o file.

#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include "switchbox_client.h"

extern SBMessage *make_message (char *line);
extern void print_message (SBMessage *m);
extern void free ();

// Returns global memory.  Don't hold on to it, just use it then forget about
// it. This only accepts lines with length <= 256, otherwise we EXIT!
char *get_line () {
  static char buffer [256];
  char *result = fgets (buffer, 256, stdin);
  // otherwise strlen (NULL) will segfault
  if (!result) return NULL;
  int len = strlen (result);
  // Make sure the line wan't too long
  assert ("What the fuck to you think you're doing?  That line is too long!"
          && result [len - 1] == '\n');
  result [len - 1] = '\0'; // kill the newline
  return result; }

/// TODO This is almost identical to the one in switchbox-cat
void *stdin_to_switchbox (void *socket_v) {
  int socket = * (int*) socket_v;
  char *line;
  while ((line = get_line ())) {
    SBMessage *m = make_message (line);
    if (!switchbox_send (socket, m)) { free (m); break; }
    free (m); }
  fflush (stdout);
  pthread_exit (NULL); }

void *switchbox_to_stdout (void *socket_v) {
  int socket = * (int*) socket_v;
  SBMessage *m;
  while ((m = switchbox_receive (socket)))
    print_message (m);
  pthread_exit (NULL); }

int main (int num_args, char **args) {
  assert ("usage: switchbox-connect hostname port"
         && num_args == 3);
  const char *hostname = args [1];
  const int port = atoi (args [2]);
  int socket = open_connection (hostname, port);
  assert ("Couldn't connect to switchbox"
         && valid_socket (socket));
  pthread_t in, out;
  pthread_create (&out, NULL, stdin_to_switchbox, &socket);
  pthread_create (&in, NULL, switchbox_to_stdout, &socket);
  pthread_join (out, NULL);
  close_connection (socket);
  pthread_join (in, NULL);
  return 0; }
