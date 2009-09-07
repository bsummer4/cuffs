%module switchbox_client
%include "cpointer.i"
%pointer_class(int, intp);
%{
#include <stdbool.h>

int open_connection(const char* hostname, const int port);
int make_listener(int port);
int accept_connection(int);
void close_connection(int s);
bool valid_socket(int s);
bool switchbox_send_string(int s,
                           int size, int type, int from, int to,
                           char *string);

char* switchbox_receive_string(int s,
                              int* size, int* type, int* from, int* to);
%}

%inline %{
// The point of this is to tell the python interpreter to release its
// global lock so that we can call a blocking c-function without
// blocking all threads.
// TODO This is a terrible hack, but I couldn't find any other way.
char *switchbox_receive_string_safe(int s,
                                    int* size,
                                    int* type,
                                    int* from,
                                    int* to)
{ char *result;
  Py_BEGIN_ALLOW_THREADS
    result = switchbox_receive_string(s, size, type, from, to);
  Py_END_ALLOW_THREADS
  return result; }
%}



bool valid_socket(int s);
int open_connection(const char* hostname, const int port);
int make_listener(int port);
int accept_connection(int);
void close_connection(int s);
bool switchbox_send_string(int s,
                           int size, int type, int from, int to,
                           char *string);

char *switchbox_receive_string(int s,
                               int* size, int* type, int* from, int* to);

char *switchbox_receive_string_safe(int s,
                                    int* size,
                                    int* type,
                                    int* from,
                                    int* to);
