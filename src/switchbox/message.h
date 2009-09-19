#pragma once

#ifdef __cplusplus
extern "C"{
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef char byte;
typedef int Listener;
typedef int Socket;

typedef struct message {
  int size;
  char data[];
} Message;

//General Info
bool streq(char *s1, char *s2);
#define iter(var, from, to) for (int var = from; var < to; var++)
extern bool debug;

Socket open_connection(const char* hostname, const int port);
Listener make_listener(int port);
Socket accept_connection(Listener);
void close_connection(Socket s);
bool valid_socket(Socket);

bool send_message(Socket, Message*);
Message* receive_message(Socket);
bool send_array(Socket, int length, byte* array);
bool send_string(Socket, char* string);
bool write_bytes(byte* output, size_t length, Socket s);
bool read_bytes(byte *input, size_t length, Socket s);

void test_message_c();


#ifdef __cplusplus
}
#endif
