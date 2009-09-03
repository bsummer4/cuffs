#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef char byte;
typedef FILE* Socket;
typedef int Listener;

// General shit
// TODO move to a different file
bool streq(char *s1, char *s2);
#define iter(var, from, to) for (int var = from; var < to; var++)

Socket open_connection(const char* hostname, const int port);
Listener make_listener(int port);
Socket accept_connection(Listener);

bool send_string(Socket, char* string);
bool send_array(Socket, int num_bytes, byte* array);
bool read_array(Socket, int *length, byte **array);

void test_message_c();
