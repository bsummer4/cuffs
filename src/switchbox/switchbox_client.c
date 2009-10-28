#include "switchbox_client.h"

// Constructing
// ------------

SBMessage *message_with_unset_data(int size, int from, int to, int type) {
  SBMessage *result = malloc(size);
  result->size = size;
  result->routing_type = type;
  result->from = from;
  result->to = to;
  return result;
}

SBMessage *message(int size, int from, int to, int type, char *data) {
  SBMessage *result = message_with_unset_data(size, from, to, type);
  memcpy(&result->data, data, size - SBMESSAGE_HEADER_SIZE);
  return result;
}

SBMessage *make_group(int from, int group_id, int num_clients, int *clients) {
  size_t header_size = SBMESSAGE_HEADER_SIZE;
  size_t admin_header_size = ADMIN_HEADER_SIZE;
  size_t clients_size = num_clients * sizeof(int);
  size_t size = header_size + admin_header_size + clients_size;

  SBMessage *result = message_with_unset_data(size, from, -1, ADMIN);
  admin_message *admin = (admin_message*) result->data;
  admin->task = DEFINE_GROUP;
  admin->group_number = group_id;
  memcpy(&admin->clients, clients, clients_size);

  return result;
}

SBMessage *remove_group(int from, int group_id) {
  size_t header_size = SBMESSAGE_HEADER_SIZE;
  size_t admin_header_size = ADMIN_HEADER_SIZE;
  size_t size = header_size + admin_header_size;

  SBMessage *result = message_with_unset_data(size, from, -1, ADMIN);
  admin_message *admin = (admin_message*) result->data;
  admin->task = DELETE_GROUP;
  admin->group_number = group_id;

  return (SBMessage *) result;
}

SBMessage *string_to_message(int type, int from, int to, char *string) {
  int size = strlen(string) + SBMESSAGE_HEADER_SIZE;
  return message(size, from, to, type, string);
}

// Accessors
// ---------

/// This returns a pointer to the inside of the message.  You should
/// probably use copy_message_string() instead.
char *message_data(SBMessage * m) {
  return m->data;
}

int message_from(SBMessage * m) {
  return m->from;
}

int message_to(SBMessage * m) {
  return m->to;
}

int message_type(SBMessage * m) {
  return m->routing_type;
}

void free_message(SBMessage * m) {
  free(m);
}

char *copy_message_string(SBMessage * m) {
  return strdup(message_data(m));
}

int announcement_code(SBMessage * m) {
  return *((int *) m->data);
}

// Transfering
// -----------

bool switchbox_send(Socket s, SBMessage * m) {
  if (!m || (unsigned) m->size < (sizeof(int) * 4)) {
    if (debug) printf("switchbox_send:  bad message.  \n");
    return false;
  }
  if (debug)
    printf("switchbox_send:  %d bytes to [socket 0x%x]\n",
           m->size, (unsigned) s);
  return send_message(s, (Message *) m);
}

/// Return NULL if the socket is invalid, fails, or is closed.  Data is
/// always null-terminated.
SBMessage *switchbox_receive(Socket s) {
  SBMessage *result = (SBMessage *) receive_message(s);
  if (!result)
    return NULL;
  if (debug)
    printf("switchbox_receive:  %d bytes from [socket 0x%x]\n",
           result->size, (unsigned) s);
  if ((unsigned) result->size < (sizeof(int) * 4)) {
    free(result);
    return NULL;
  }
  return result;
}
