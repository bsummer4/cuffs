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
