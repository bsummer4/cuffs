#include "switchbox_client.h"
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>

#define MAX_CLIENTS 1024
#define MAX_GROUPS 64

void run_switchbox(int port);
void send_hello_message(Socket, int);
void announce(int code, int client_id);
bool switchbox_locking_send(SBMessage* m);

// ==============
// Implementation
// ==============

int main (int argc, char **argv) {
  // A broken pipe should just cause read() and write() to fail.
  signal(SIGPIPE, SIG_IGN);
  run_switchbox(SWITCHBOX_PORT);
  return 0; }


char *routing_type_to_string(int routing_type) {
  switch (routing_type) {
   case UNICAST: return "unicast";
   case BROADCAST: return "broadcast";
   case MULTICAST: return "multicast";
   case ADMIN: return "admin";
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

// TODO used might not be initialized to false.  Either initialize the
// entire array or keep track of what's been touched.
typedef struct group{
  int num_members;
  int *members;
  bool used;
} Group;

// The 'clients' array stores all clients in the system.  The clients
// 'id' corresponds to their index in this array.
Client clients[MAX_CLIENTS];

Group multicast_groups[MAX_GROUPS];

// The index of the first unused and last used slot in the 'clients' array.
int first_free_client= 0;
int last_used_client = -1;

bool is_client_used(Client *client){ return client->used;}

// Moves 'first_free_client forward until it points to a free slot
void update_free_client() {
  while (is_client_used(clients + first_free_client)) first_free_client++;}

// Index is the of a new client.  If it's after 'last_used_client',
// then change it.
//
// use index = -1 if a client was removed.  This moves
// last_used_client back until it points to a real client.
void update_last_used_client(int index) {
  if (index == -1)
    while ((last_used_client >= 0) &&
           !is_client_used(clients + last_used_client))
      last_used_client--;
  else if (last_used_client < index) last_used_client = index; }

// Returns NULL if there isn't room for another client.
Client *get_new_client(Socket s) {
  if (first_free_client >= MAX_CLIENTS) return NULL;
  int index = first_free_client;
  Client *result = clients + index;
  result->connection = s;
  result->used = true;
  pthread_mutex_init(&(result->lock), NULL);
  update_free_client();
  update_last_used_client(index);
  return result;
}

bool remove_client(Client *client) {
  int index = client-clients;
  if ((!client) || !is_client_used(client)) return false; 
  if (debug) printf("Lost connection from client %d\n", index);

  // TODO this check should be in close_connection()
  if (!valid_socket(client->connection)) return false; 
  close_connection(client->connection);

  pthread_mutex_destroy(&client->lock);
  bzero(client, sizeof(Client));
  
  if (index == last_used_client) update_last_used_client(-1);
  if (first_free_client > index) first_free_client = index;
  
  return true;
}


void send_error(int to, int type){
  if (debug) printf("sending error %d to %d\n", type, to);
  SBMessage * msg = malloc(sizeof(int)*4);
  msg->size = sizeof(int)*4;
  msg->routing_type = type;
  msg->from = -1;
  msg->to   = to;
  switchbox_locking_send(msg);
}

// -----------
// Connections
// -----------

// Free the members array, and set group.used to false
bool delete_group(int group_id) {
  if (debug) printf("deleting group %d\n", group_id);
  if (group_id >= MAX_GROUPS) return false;
  Group *group = multicast_groups + group_id;
  if (!group->used) return false;
  if (group->num_members != 0) free(group->members);
  group->used = false;
  return true;
}

// As long as group_id is valid, shit will work.
bool define_group(int group_id, int num_clients, int *users) {
  if (debug) {
    printf("defining group %d with users [", group_id);
    iter(ii, 0, num_clients) printf("%d ", users[ii]);
    printf("]\n");
  }
      
  if (group_id >= MAX_GROUPS) return false;
  Group *group = multicast_groups + group_id;
  if (group->used) delete_group(group_id);
  group->used = true;
  group->num_members = num_clients;
  group->members = users;
  return true;
}


bool switchbox_handle_admin(SBMessage * m){
  if (debug) printf("admin message: [");
  if (debug) iter(ii, 0, m->size) printf("%x ", (unsigned)((char*)m)[ii]);
  if (debug) printf("]\n");

  admin_message *adm = (admin_message*) m->data;

  size_t admin_message_size = m->size - sizeof(int)*4;
  size_t clients_size = admin_message_size - (sizeof(admin_task_t) + sizeof(int));
  assert(clients_size % sizeof(int) == 0);

  int num_clients = clients_size / sizeof(int);
  int gn = adm->group_number;
  int* clients = malloc(sizeof(int) * num_clients);
  memcpy(clients, adm->clients, sizeof(int) * num_clients);

  if (debug) {
    printf("  task: %d\n", adm->task);
    printf("  group_id: %d\n", gn);
    printf("  num_clients: %d\n", num_clients);
  }

  switch (adm->task) {
  case DEFINE_GROUP:
    send_error(m->from,
               define_group(gn, num_clients, clients) ? ADMIN_SUCCESS : ADMIN_FAIL);
    break;

  // We don't care if delete_group fails.  It doesn't hurt anything.  
  case DELETE_GROUP: delete_group(gn); break;
  default: send_error(m->from, ADMIN_FAIL);
  }
}

// Locks the target client until it finishes sending the message.
bool switchbox_locking_send(SBMessage* m) {
  Client *target = clients + m->to;
  Socket socket = target->connection;
  if (!is_client_used(target)) return false;
  if (debug)
    printf("switchbox_locking_send: Locking before sending message to %d.  \n",
           m->to);
  pthread_mutex_lock(&target->lock);
  bool success = switchbox_send(socket, m);
  if (debug) printf("switchbox_locking_send: [%d] Sent, unlocking... \n",
                    m->to);
  pthread_mutex_unlock(&target->lock);
  return success; }

bool multicast(SBMessage *m) {
  if (debug) printf("multicasting to %d\n", m->to);
  int group_id = m->to;
  if (debug && group_id >= MAX_GROUPS) printf("MULTICAST ERROR: group number isn't even resonable\n");
  if (group_id >= MAX_GROUPS) return false;
  Group *g = multicast_groups + group_id;
  if (debug && !g->used) printf("MULTICAST ERROR: dude, group %d isn't defined\n", group_id);
  if (!g->used) return false;
  iter(ii, 0, g->num_members) {
    m->to = g->members[ii];
    if (!switchbox_locking_send(m)) {
      if (debug) printf("MULTICAST ERROR: couldn't send to %d\n", m->to);
      return false;
    }
  }
  return true;
}

bool broadcast(SBMessage *m) {
  if (debug) printf("broadcast from %d\n", m->from);
  iter(ii, 0, last_used_client+1) {
    if (debug && is_client_used(clients + ii))
      if (debug) printf("broadcasting to %d\n", ii);
    m->to = ii;
    if (!switchbox_locking_send(m)) return false; }
  return true; }

void announce(int code, int client_id) {
  int size = sizeof(int) * 5;
  SBMessage *m = malloc(size);
  m->size = size;
  m->from = client_id;
  m->to = -1;
  m->routing_type = ANNOUNCE;
  memcpy(m->data, &code, sizeof(code));
  broadcast(m);
  free(m);
}

void print_message(SBMessage *m);

void *handle_connection(void *client_) {
  Client* client = client_;
  SBMessage *m;
  int client_id = client - clients;
  send_hello_message(client->connection, client_id);
  // announce(CONNECTION, client_id);
  while ((m = switchbox_receive(client->connection))) {
    m->from = client_id; // Client can't lie about its identity.
    if (debug) {
      printf ("handle_connection: new message [%s][%d bytes] #%d -> #%d.  \n",
              routing_type_to_string(m->routing_type),
              m->size, m->from, m->to);
      print_message(m);
    }
    switch (m->routing_type) {
    case UNICAST:
      if (!switchbox_locking_send(m)) {
        if (debug)
          printf ("handle_connection: ERROR bad client %d (disconnected?).  \n", m->to);
        send_error(client_id, INVALID_TARGET);
      }
      break;
    case BROADCAST: assert(broadcast(m)); break;
    case MULTICAST:
      if (!multicast(m)) send_error(client_id, INVALID_TARGET); break;
    case ADMIN: switchbox_handle_admin(m); break;
    default: send_error(client_id, TYPE_ERROR);
    }
    free(m); }

  // The connection has been closed.
  remove_client(client);
  return NULL; }

void setup_connection(Socket s) {
  Client *client = get_new_client(s);
  if (debug)
    printf("setup_connection: assigning client #%d [socket 0x%x].  \n",
           client - clients, (unsigned) s);
  pthread_create(&(client->thread), NULL, handle_connection, client); }

void run_switchbox(int port) {
  Listener l = make_listener(port);
  Socket s;
  iter (ii, 0, MAX_GROUPS) multicast_groups[ii].used = false;
  while (valid_socket(s = accept_connection(l))) setup_connection(s); }

void send_hello_message(Socket s, int addr){
    SBMessage * msg = malloc(sizeof(int)*4);
    msg->size = sizeof(int)*4;
    msg->routing_type = ADMIN;
    msg->from = addr;
    msg->to   = addr;
    switchbox_locking_send(msg);
}

void print_message(SBMessage *m) {
  printf("  Message: {type: %s, size: %d, from: %d, to: %d, data: [", routing_type_to_string(m->routing_type),
         m->size, m->from, m->to);
  iter(ii, 0, m->size) printf("%x ", (unsigned)(m->data[ii]));
  printf("]}\n");
}
