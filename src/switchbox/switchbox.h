#include "message.h"

enum { BROADCAST_TARGET, SWITCHBOX_TARGET};

typedef struct message{
  int length;
  int type;
  int from_addr; //from address
  int to_addr;   //to address
} Message;


bool switchbox_send(Message*, int num_bytes, byte* array);
bool switchbox_receive(Message*); 
