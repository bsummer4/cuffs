#include "switchbox_client.h"
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>

#define MAX_CLIENTS 1024
#define MAX_GROUPS 64

enum { NEW_CLIENT, DELETED_CLIENT };

void run_switchbox(int port);
void announce(SBMessage *admin_message);
bool switchbox_locking_send(SBMessage * m);
void print_message(SBMessage * m);

// ==============
// Implementation
// ==============

int main(int argc, char **argv) {
  // A broken pipe should just cause read() and write() to fail.
  signal(SIGPIPE, SIG_IGN);
  run_switchbox(SWITCHBOX_PORT);
  return 0;
}


char *routing_type_to_string(int routing_type) {
  switch (routing_type) {
  case UNICAST:
    return "unicast";
  case BROADCAST:
    return "broadcast";
  case MULTICAST:
    return "multicast";
  case ADMIN:
    return "admin";
  case ANNOUNCE:
    return "announce";
  }

  return "(other or invalid)";
}


// -----------------------------
// Clients and the clients array
// -----------------------------

typedef struct client {
  Socket connection;
  pthread_t thread;
  pthread_mutex_t lock;
  bool used;
} Client;

typedef struct group {
  int num_members;
  int *members;
  bool used;
} Group;

// The 'clients' and 'groups' arrays store all clients and groups in
// the switchbox.  The 'id' of a client or group corresponds to their
// index in this array.
Client clients[MAX_CLIENTS];
Group groups[MAX_GROUPS];

// The index of the first unused and last used slot in the 'clients'
// array.  We use this so we can efficiently find a empty slot when
// someone joins while always assigning the lowest available address
// number.
int first_free_client = 0;
int last_used_client = -1;

bool is_client_used(Client * client) {
  return client->used;
}

bool valid_client_address(int index) {
  return index >= 0 && index < MAX_CLIENTS;
}

bool valid_group_address(int index) {
  return index >= 0 && index < MAX_GROUPS;
}

// Moves 'first_free_client forward until it points to a slot that is
// unused.  This needs to be called whenever the first_free_client
// gets used
void update_free_client() {
  while (is_client_used(clients + first_free_client))
    first_free_client++;
}

// Index is the of a new client.  If it's after 'last_used_client',
// then change it.
//
// use index = -1 if a client was removed.  This moves
// last_used_client back until it points to a real client.
void update_last_used_client(int what_changed, int index) {
  switch (what_changed) {
  case NEW_CLIENT:
    if (last_used_client < index)
      last_used_client = index;
    break;
  case DELETED_CLIENT:
    while ((last_used_client >= 0) &&
           !is_client_used(clients + last_used_client))
      last_used_client--;
  }
}

// Returns NULL if there isn't room for another client.
Client *get_new_client(Socket s) {
  if (!valid_client_address(first_free_client))
    return NULL;
  int index = first_free_client;
  Client *result = clients + index;
  result->connection = s;
  result->used = true;
  pthread_mutex_init(&(result->lock), NULL);
  update_free_client();
  update_last_used_client(NEW_CLIENT, index);
  return result;
}

bool remove_client(Client * client) {
  int index = client - clients;
  if ((!client) || !is_client_used(client))
    return false;
  if (debug)
    printf("Lost connection from client %d\n", index);

  /// @TODO this check should be in close_connection()
  if (!valid_socket(client->connection))
    return false;

  close_connection(client->connection);

  pthread_mutex_destroy(&client->lock);
  bzero(client, sizeof(Client));

  if (index == last_used_client)
    update_last_used_client(DELETED_CLIENT, -1);
  if (first_free_client > index)
    first_free_client = index;

  return true;
}


void send_error(int to, int type) {
  if (debug)
    printf("sending error %d to %d\n", type, to);
  SBMessage *msg = malloc(sizeof(int) * 4);
  msg->size = sizeof(int) * 4;
  msg->routing_type = type;
  msg->from = -1;
  msg->to = to;
  switchbox_locking_send(msg);
  free(msg);
}

// -----------
// Connections
// -----------

// Free the members array, and set group.used to false
bool delete_group(int group_id) {
  if (debug)
    printf("deleting group %d\n", group_id);
  if (!valid_group_address(group_id))
    return false;
  Group *group = groups + group_id;
  if (!group->used)
    return false;
  if (group->num_members != 0)
    free(group->members);
  group->used = false;
  return true;
}

// As long as group_id is valid, shit will work.
bool define_group(int group_id, int num_clients, int *users) {
  if (debug) {
    printf("Defining group %d with users [", group_id);
    iter(ii, 0, num_clients) printf("%d ", users[ii]);
    printf("]\n");
  }

  if (!valid_group_address(group_id))
    return false;
  Group *group = groups + group_id;
  if (group->used)
    delete_group(group_id);     // We'll replace it
  group->used = true;
  group->num_members = num_clients;
  group->members = users;
  return true;
}


void switchbox_handle_admin(SBMessage * m) {
  admin_message *admin = (admin_message *) m->data;
  size_t header_size = sizeof(int) * 4;
  size_t admin_size = sizeof(admin_task_t) + sizeof(int);
  size_t clients_size = m->size - (header_size + admin_size);
  if (clients_size % sizeof(int) != 0)
    goto admin_fail;

  int num_clients = clients_size / sizeof(int);
  int group_id = admin->group_number;
  int *clients = malloc(sizeof(int) * num_clients);
  memcpy(clients, admin->clients, sizeof(int) * num_clients);

  if (debug)
    printf("admin message: {task: %d, group_id: %d, num_clients: %d\n",
           admin->task, group_id, num_clients);

  bool success = false;
  switch (admin->task) {
  case DEFINE_GROUP:
    success = define_group(group_id, num_clients, clients);
    break;
  case DELETE_GROUP:
    success = delete_group(group_id);
    break;
  case NEW_CONNECTION:
  case LOST_CONNECTION:
  default:
    return;
  }

  if (success) {
    announce(m);
    return;
  }

 admin_fail:
  send_error(m->from, ADMIN_FAIL);
}

// Locks the target client until it finishes sending the message.
bool switchbox_locking_send(SBMessage * m) {
  Client *target = clients + m->to;
  Socket socket = target->connection;
  if (!is_client_used(target))
    return false;
  if (debug) {
    printf
      ("switchbox_locking_send: Locking before sending message to %d.  \n",
       m->to);
    print_message(m);
  }
  pthread_mutex_lock(&target->lock);
  bool success = switchbox_send(socket, m);
  if (debug)
    printf("switchbox_locking_send: [%d] Sent, unlocking... \n", m->to);
  pthread_mutex_unlock(&target->lock);
  return success;
}

bool multicast(SBMessage * m) {
  if (debug)
    printf("multicasting to %d\n", m->to);
  int group_id = m->to;
  Group *g = groups + group_id;

  if (!valid_group_address(group_id)) {
    if (debug)
      printf("MULTICAST ERROR: group number isn't even resonable\n");
    return false;
  }
  if (!g->used) {
    if (debug)
      printf("MULTICAST ERROR: dude, group %d isn't defined\n", group_id);
    return false;
  }

  iter(ii, 0, g->num_members) {
    m->to = g->members[ii];
    if (!switchbox_locking_send(m)) {
      if (debug)
        printf("MULTICAST ERROR: couldn't send to %d\n", m->to);
      return false;
    }
  }
  return true;
}

bool broadcast(SBMessage * m) {
  if (debug)
    printf("broadcast from %d\n", m->from);
  iter(ii, 0, last_used_client + 1) {
    if (debug && is_client_used(clients + ii))
      if (debug)
        printf("broadcasting to %d\n", ii);
    m->to = ii;
    if (!switchbox_locking_send(m))
      return false;
  }
  return true;
}

void announce(SBMessage *admin_message) {
  admin_message->routing_type = ANNOUNCE;
  broadcast(admin_message);
}

void *handle_connection(void *client_) {
  Client *client = client_;
  SBMessage *m;
  int client_id = client - clients;

  // Announce that the user has connected
  m = message_with_unset_data(SBMESSAGE_HEADER_SIZE + ADMIN_HEADER_SIZE,
                              client_id, -1, ANNOUNCE);
  ((admin_message *) m->data)->task = NEW_CONNECTION;
  ((admin_message *) m->data)->group_number = -1;
  announce(m);
  free(m);

  while ((m = switchbox_receive(client->connection))) {
    m->from = client_id;        // Client can't lie about its identity.
    if (debug) {
      printf("handle_connection: new message [#%d -> #%d].\n",
             m->from, m->to);
      print_message(m);
    }
    switch (m->routing_type) {
    case UNICAST:
      if (!switchbox_locking_send(m)) {
        if (debug)
          printf
            ("handle_connection: ERROR bad client %d (disconnected?).  \n",
             m->to);
        send_error(client_id, INVALID_TARGET);
      }
      break;
    case BROADCAST:
      assert(broadcast(m));
      break;
    case MULTICAST:
      if (!multicast(m))
        send_error(client_id, INVALID_TARGET);
      break;
    case ADMIN:
      switchbox_handle_admin(m);
      break;
    default:
      send_error(client_id, TYPE_ERROR);
    }
    free(m);
  }


  // The connection has been closed
  remove_client(client);

  // Announce that the user has disconnected
  m = message_with_unset_data(SBMESSAGE_HEADER_SIZE + ADMIN_HEADER_SIZE,
                              client_id, -1, ANNOUNCE);
  ((admin_message *) m->data)->task = LOST_CONNECTION;
  ((admin_message *) m->data)->group_number = -1;
  announce(m);
  free(m);

  return NULL;
}

void setup_connection(Socket s) {
  Client *client = get_new_client(s);
  if (debug)
    printf("setup_connection: assigning client #%d [socket 0x%x].  \n",
           client - clients, (unsigned) s);
  pthread_create(&(client->thread), NULL, handle_connection, client);
}

void run_switchbox(int port) {
  Listener l = make_listener(port);
  Socket s;
  iter(ii, 0, MAX_GROUPS) groups[ii].used = false;
  while (valid_socket(s = accept_connection(l)))
    setup_connection(s);
}

// This prints everything about a message to standard output.  It is
// for debugging.
void print_message(SBMessage * m) {
  printf("  Message: {type: %s, size: %d, from: %d, to: %d, data: [",
         routing_type_to_string(m->routing_type), m->size, m->from, m->to);
  int data_size = m->size - SBMESSAGE_HEADER_SIZE;
  iter(ii, 0, data_size) printf("%x ", (unsigned) m->data[ii]);
  printf("]}\n");
}
