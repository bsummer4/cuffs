#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef char byte;
typedef FILE* Socket;

bool streq(char *s1, char *s2);
#define iter(var, from, to) for (int var = from; var < to; var++)

Socket open_connection(const char* hostname, const int port);
Socket make_listener(int port);
Socket accept_connection(Socket);

bool send_string(Socket, char* string);
bool send_array(Socket, int num_bytes, byte* array);
bool read_array(Socket, int *length, byte **array);

void test_message_c();
