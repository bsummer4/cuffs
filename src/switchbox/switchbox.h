#include "socket.h"

enum { BROADCAST_TARGET, SWITCHBOX_TARGET};

typedef struct connection {
  Socket socket;
  int target;
} Connection;

bool switchbox_send(Connection, int num_bytes, byte* array);
bool switchbox_send_string(Connection, char* string);
