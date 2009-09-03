#include "message.h"

// values for Message.type
enum { UNICAST, BROADCAST, TYPE_ERROR, FROM_ADDRESS_ERROR, TO_ADDRESS_ERROR};

typedef struct sb_message {
  int length;
  int type;
  int from_address;
  int to_address;
  char message[];
} SBMessage;

bool switchbox_send(Socket, SBMessage*);
SBMessage *switchbox_receive(Socket);
