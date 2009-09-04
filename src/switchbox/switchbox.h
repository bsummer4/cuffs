#include "message.h"

// values for Message.type
enum { UNICAST, BROADCAST, TYPE_ERROR, FROM_ADDRESS_ERROR, TO_ADDRESS_ERROR};

typedef struct switchbox_message {
  int size;
  int routing_type;
  int from;
  int to;
  char data[];
} SBMessage;

bool switchbox_send(Socket, SBMessage*);
SBMessage *switchbox_receive(Socket);
