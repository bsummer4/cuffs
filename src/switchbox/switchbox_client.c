#include "switchbox_client.h"

bool switchbox_send_string(Socket s,
                           int size, int type, int from, int to,
                           char *string) {
  SBMessage *result = malloc(size);
  result->size = size;
  result->routing_type = type;
  result->from = from;
  result->to = to;
  memcpy(&result->data, string, size - 4 * sizeof(int));
  return switchbox_send(s, (SBMessage*) result);
}

char *switchbox_receive_string(Socket s,
                              int* size, int* type, int* from, int* to)
{
  SBMessage *m = switchbox_receive(s);
  if (!m) return NULL;
  *size = m->size;
  *from = m->from;
  *to = m->to;
  *type = m->routing_type;
  int string_size = m->size - (4*sizeof(int)) ;
  char *result = malloc(string_size + 1);
  memcpy(result, m->data, string_size);
  result[string_size] = '\0';
  free(m);
  return result;
}

bool switchbox_send(Socket s, SBMessage* m) {
  if (!m) return false;
  if ((unsigned) m->size < (sizeof(int) * 4)) return false;
  if (debug) printf("switchbox_send:  %d bytes to [socket 0x%x]\n",
                    m->size, (unsigned) s);
  return send_message(s, (Message*) m); }

// Return NULL if the socket is invalid, fails, or is closed.
SBMessage *switchbox_receive(Socket s) {
  SBMessage *result = (SBMessage*) receive_message(s);
  if (!result) return NULL;
  if (debug) printf("switchbox_receive:  %d bytes from [socket 0x%x]\n",
                    result->size, (unsigned) s);
  if ((unsigned) result->size < (sizeof(int) * 4)) {
    free(result); return NULL; }
  return result; }
