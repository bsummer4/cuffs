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
typedef enum { 
  UNICAST, 
  BROADCAST, 
  MULTICAST,
  ANNOUNCE,
  ADMIN,
  TYPE_ERROR, 
  INVALID_TARGET, 
  ADMIN_SUCCESS,
  ADMIN_FAIL
} message_type;

typedef enum { 
  DEFINE_GROUP,
  DELETE_GROUP
} admin_task_t;
  
typedef struct {
  admin_task_t task;
  int group_number;
  int clients[];
} admin_message;

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
