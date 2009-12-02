%module switchbox_client

%{
#include <stdbool.h>
#include "switchbox_client.h"
%}

%inline %{
  // The point of this is to tell the python interpreter to release its
  // global lock so that we can call a blocking c-function without
  // blocking all threads.
  SBMessage * switchbox_receive_safe(int s)
  {
    SBMessage *result;
    Py_BEGIN_ALLOW_THREADS
      result = switchbox_receive(s);
    Py_END_ALLOW_THREADS
    return result;
  }

  bool switchbox_send_safe(int s, SBMessage * m)
  {
    bool result;
    Py_BEGIN_ALLOW_THREADS
      result = switchbox_send(s, m);
    Py_END_ALLOW_THREADS
    return result;
  }
%}

bool valid_socket(int s);
int open_connection(const char* hostname, const int port);
int make_listener(int port);
int accept_connection(int);
void close_connection(int s);

char * copy_message_string(SBMessage * m);
SBMessage * string_to_message (int type, int from, int to, char *string);

char * message_data(SBMessage * m);
int message_from(SBMessage * m);
int message_to(SBMessage * m);
int message_type(SBMessage * m);
int announcement_code (SBMessage * m);
void free_message(SBMessage * m);
