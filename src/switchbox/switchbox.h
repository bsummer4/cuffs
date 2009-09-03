#include "message.h"

// values for Message.type
enum { UNICAST, BROADCAST};

typedef struct message {
  int length;
  int type;
  int from_address;
  int to_address;
  char message[];
} Message;

bool switchbox_send(Socket, Message*);
Message *switchbox_receive(Socket);
