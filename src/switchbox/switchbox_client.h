#pragma once

#ifdef __cplusplus
extern "C"{
#endif

#include "message.h"

#define SWITCHBOX_PORT 80021

typedef struct switchbox_message {
  int size;
  int routing_type;
  int from;
  int to;
  char data[];
} SBMessage;

// values for Message.type
enum { UNICAST, BROADCAST, TYPE_ERROR, FROM_ADDRESS_ERROR, TO_ADDRESS_ERROR, SWITCHBOX_ANNOUNCE};

bool switchbox_send(Socket, SBMessage*);
SBMessage *switchbox_receive(Socket);

bool switchbox_send_string(Socket s,
                           int size, int type, int from, int to,
                           char *string);

char *switchbox_receive_string(Socket s,
                               int* size, int* type, int* from, int* to);

#ifdef __cplusplus
}
#endif
