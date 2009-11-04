/**
   # Switchbox Client Library
   An api for sending and receiving messages to the switchbox.
   Clients should never manipulate messages directly.
 */

#pragma once
#include "message.h"

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

/// The format of all messages that are sent to, and received by the
/// switchbox
typedef struct switchbox_message {
  int size;
  message_type_t routing_type;
  int from;
  int to;
  char data[];
} SBMessage;

#define SBMESSAGE_HEADER_SIZE 4*sizeof(int)

typedef enum {
  DEFINE_GROUP,
  DELETE_GROUP,
  NEW_CONNECTION,
  LOST_CONNECTION
} admin_task_t;

/// The layout of SBMessage.data when SBMessage.routing_type == ADMIN
typedef struct {
  admin_task_t task;
  int group_number;
  int clients[];
} admin_message;

#define ADMIN_HEADER_SIZE 2*sizeof(int)

// Transmitting
bool switchbox_send(Socket, SBMessage *);
SBMessage *switchbox_receive(int);

// Constructing
SBMessage *string_to_message(int type, int from, int to, char *string);
SBMessage *remove_group(int from, int group_id);
SBMessage *make_group(int from, int group_id, int num_clients, int *clients);
SBMessage *message(int size, int from, int to, int type, char *data);
SBMessage *message_with_unset_data(int size, int from, int to, int type);

// Accessors
char *message_data(SBMessage * m);
int message_from(SBMessage * m);
int message_to(SBMessage * m);
int message_type(SBMessage * m);
void free_message(SBMessage * m);
char *copy_message_string(SBMessage * m);
int announcement_code(SBMessage * m);
