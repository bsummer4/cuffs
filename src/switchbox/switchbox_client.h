/**
   # Switchbox Client Library
   An api for sending and receiving messages to the switchbox.  You
   should never manipulate messages directly.
 */

#pragma once

#include "message.h"

#define SWITCHBOX_PORT 80044

typedef enum {
  UNICAST,
  BROADCAST,
  MULTICAST,
  ANNOUNCE,
  ADMIN,
  TYPE_ERROR,
  INVALID_TARGET,
  ADMIN_FAIL
} message_type_t;

typedef enum {
  DEFINE_GROUP,
  DELETE_GROUP,
  NEW_CONNECTION,
  LOST_CONNECTION
} admin_task_t;

typedef struct switchbox_message {
  int size;
  message_type_t routing_type;
  int from;
  int to;
  char data[];
} SBMessage;

#define SBMESSAGE_HEADER_SIZE 4*sizeof(int)

typedef struct {
  admin_task_t task;
  int group_number;
  int clients[];
} admin_message;

#define ADMIN_HEADER_SIZE 2*sizeof(int)

bool switchbox_send(Socket, SBMessage *);
SBMessage *switchbox_receive(int);
SBMessage *string_to_message(int type, int from, int to, char *string);
SBMessage *remove_group(int from, int group_id);
SBMessage *make_group(int from, int group_id, int num_clients, int *clients);

SBMessage *message(int size, int from, int to, int type, char* data);
SBMessage *message_with_unset_data(int size, int from, int to, int type);

// Accessor Functions
char *message_data(SBMessage * m);
int message_from(SBMessage * m);
int message_to(SBMessage * m);
int message_type(SBMessage * m);
void free_message(SBMessage * m);
char *copy_message_string(SBMessage * m);
int announcement_code(SBMessage * m);
