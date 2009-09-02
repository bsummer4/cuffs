#include <message.h>
#include <string.h>

bool send_string(Socket socket, char* string) {
  return send_array(socket, strlen(string), string); }
