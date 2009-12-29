/**
@addtogroup Switchbox
@section Some pointers for reading this:
  - We use the switchbox_client, and message libraries, read those
    first.
  - Each client has its own thread: This is somewhat inefficient, but
    it's simple, and it's good enough for what we're doing.
  - New clients are always assigned the lowest available address.
  - Client and Group addresses are indexes into a big global array of
    structs.  This is simple and efficient.
  - Clients are only locked when sending them data.
  - Messages can be any size are are stored on the heap.  This is
    simple and flexible (no limit on message size).

@section Possible preformance enhancements:
  - Use select() instead of threads.
  - Use a message buffer on the stack and only malloc/free if the
    message is too big for the buffer.

  These are all complicated and probably not worth doing.

@TODO What if a member of a group dissconnects?  Should the group be
    removed?  Maybe that member should be removed from the group.
    For now sending to the group sends to a subset of the members
    and then an error is send to the sender.  This is not a very
    good solution.
*/

#include "switchbox_client.h"
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>

#define MAX_CLIENTS 1024
#define MAX_GROUPS 64

enum { NEW_CLIENT, DELETED_CLIENT };

void run(int port);
void init();
void announce(SBMessage * admin_message);
bool locking_send(SBMessage * m);
void print_message(SBMessage * m);
void handler();

// Implementation
// ==============

int main(int argc, char **argv) {
  int port = (argc <= 1) ? 0 : atoi(argv[1]);

  // A broken pipe should just cause read() and write() to fail insead
  // of stopping the program
  signal(SIGPIPE, SIG_IGN);
  signal(SIGINT, handler);
  // Catch Ctrl+c and "kill"
  signal(SIGINT, handler);
  signal(SIGTERM, handler);

  init();
  run(port);
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

// Clients and the clients array
// -----------------------------

typedef struct client {
  Socket connection;
  pthread_t thread;
  pthread_mutex_t lock;         // for sending
  bool used;
} Client;

typedef struct group {
  int num_members;
  int *members;
  bool used;
} Group;

/// The 'clients' and 'groups' arrays store all clients and groups in
/// the switchbox.  The 'id' of a client or group corresponds to their
/// index in this array.
Client clients[MAX_CLIENTS];
Group groups[MAX_GROUPS];
Socket s; ///< Global Socket so the signal handler can access the socket

/// The index of the first unused and last used slot in the 'clients'
/// array.  We use this so we can efficiently find a empty slot when
/// someone joins while always assigning the lowest available address
/// number.
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

/// Moves 'first_free_client' forward until it points to an element
/// that is unused.  This needs to be called whenever the
/// 'first_free_client' gets used.
void update_free_client() {
  while (is_client_used(clients + first_free_client))
    first_free_client++;
}

/// 'index' is the address of a new client.  Calling this whenever a
/// client is created or removed keeps 'last_used_client' valid.
/// 'what_changed' should be either NEW_CLIENT or DELETED_CLIENT.
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
    break;
  default:
    assert(false);
  }
}

/// Returns NULL if we already have MAX_CLIENTS
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
  locking_send(msg);
  free(msg);
}

// Connections
// -----------

/// Free the 'members' array, and set group.used to false.
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

/// Redefining an existing group is valid.  'users' must be on the
/// heap (it will be freed when the group is deleted); we take
/// responsibility for it.
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
    delete_group(group_id);
  group->used = true;
  group->num_members = num_clients;
  group->members = users;
  return true;
}

void handle_admin(SBMessage * m) {
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

/// Locks the m.target's client while sending.
bool locking_send(SBMessage * m) {
  Client *target = clients + m->to;
  Socket socket = target->connection;
  if (!is_client_used(target))
    return false;
  if (debug) {
    printf("locking_send: Locking before sending message to %d.  \n", m->to);
    print_message(m);
  }
  pthread_mutex_lock(&target->lock);
  bool success = switchbox_send(socket, m);
  if (debug)
    printf("locking_send: [%d] Sent, unlocking... \n", m->to);
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
    if (!locking_send(m)) {
      if (debug)
        printf("MULTICAST ERROR: couldn't send to %d\n", m->to);
      return false;
    }
  }
  return true;
}

/// Returns false if we couldn't send to one or more clients
bool broadcast(SBMessage * m) {
  bool success = true;
  if (debug)
    printf("broadcast from %d\n", m->from);
  iter(ii, 0, last_used_client + 1) {
    if (debug && is_client_used(clients + ii))
      if (debug)
        printf("broadcasting to %d\n", ii);
    m->to = ii;
    if (!locking_send(m))
      success = false; // Don't stop just because one person
                       // disconnected
  }
  return success;
}

void announce(SBMessage * admin_message) {
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
      if (!locking_send(m)) {
        if (debug)
          printf
            ("handle_connection: ERROR bad client %d (disconnected?).  \n",
             m->to);
        send_error(client_id, INVALID_TARGET);
      }
      break;
    case BROADCAST:
      /// If this fails, we know a client has disconnected.  We could
      /// try to close off the client now, but it should be handled
      /// very soon by its own thread.
      ///
      /// We also have no need to send an error to the sender since
      /// they don't really care.  They just want the message to get
      /// to everyone connected.
      broadcast(m);
      break;
    case MULTICAST:
      if (!multicast(m))
        send_error(client_id, INVALID_TARGET);
      break;
    case ADMIN:
      handle_admin(m);
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

/// Gets a new client and runs 'handle_connection()' on a new thread.
void setup_connection(Socket s) {
  Client *client = get_new_client(s);
  if (debug)
    printf("setup_connection: assigning client #%d [socket 0x%x].  \n",
           (int) (client - clients), (unsigned) s);
  pthread_create(&(client->thread), NULL, handle_connection, client);
}

void init() {
  iter(ii, 0, MAX_GROUPS) groups[ii].used = false;
  iter(ii, 0, MAX_CLIENTS) clients[ii].used = false;
}

void run(int port) {
  Listener l = make_listener(port);
  printf("listening\n");
  printf("port %d\n", listener_port(l));

  while (valid_socket(s = accept_connection(l)))
    setup_connection(s);
}


/// This prints everything about a message to standard output.  It is
/// for debugging.
/// @TODO Move this to switchbox_client.c
void print_message(SBMessage * m) {
  printf("  Message: {type: %s, size: %d, from: %d, to: %d, data: [",
         routing_type_to_string(m->routing_type), m->size, m->from, m->to);
  int data_size = m->size - SBMESSAGE_HEADER_SIZE;
  iter(ii, 0, data_size) printf("%x ", (unsigned) m->data[ii]);
  printf("]}\n");
}


/// Signal handler to cleanly shutdown.
void handler() {
  // Close main socket
  close(s);
  // Close all the connections
  for (int i = 0; i < MAX_CLIENTS; i++)
    remove_client(&clients[i]);
  exit(0);
}
