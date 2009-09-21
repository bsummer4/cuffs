#include "switchbox_client.h"
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>
#include "dllist.h"

#define MAX_CLIENTS 1024
#define MAX_GROUPS 64

void run_switchbox(int port);
void send_hello_message(Socket, int);
void announce(int code, int client_id);


// ==============
// Implementation
// ==============

int main (int argc, char **argv) {
  // A broken pipe should just cause read() and write() to fail.
  signal(SIGPIPE, SIG_IGN);
  run_switchbox(SWITCHBOX_PORT);
  return 0; }


// -----------------------------
// Clients and the clients array
// -----------------------------

typedef struct client {
  Socket connection;
  pthread_t thread;
  pthread_mutex_t lock;
  bool used;
} Client;

typedef struct group{
  Dllist members;
  bool active;
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


// -----------
// Connections
// -----------

bool switchbox_handle_admin(SBMessage * m){
    admin_message * adm = (admin_message*)m->data;
    int cl_length = m->size - sizeof(int)*4 - sizeof(admin_task_t) - sizeof(int);
    int gn = adm->group_number;
    int* list = adm->clients;
    switch (adm->task){
    case ADD_TO_GROUP:
        for (int i = 0; i < cl_length; i++){
            dll_append(multicast_groups[gn].members, new_jval_i(list[i]));
        }
        multicast_groups[gn].active == true;
        break;
    case RM_FROM_GROUP:
        break;
    case CREATE_GROUP:
        break;
    case DELETE_GROUP:
        break;
    default:
        break;
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

bool broadcast(SBMessage *m) {
  iter(ii, 0, last_used_client+1) {
    if (debug && is_client_used(clients + ii))
      printf("broadcasting to %d\n", ii);
    m->to = ii;
    if (!switchbox_locking_send(m))
      return false; }}

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

void *handle_connection(void *client_) {
  Client* client = client_;
  SBMessage *m;
  int client_id = client - clients;
  send_hello_message(client->connection, client_id);
  // announce(CONNECTION, client_id);
  while ((m = switchbox_receive(client->connection))) {
    m->from = client_id; // Client can't lie about its identity.
    if (debug)
      printf ("handle_connection: new message [%s][%d bytes] #%d -> #%d.  \n",
              (m->routing_type ? "broadcast" : "unicast"),
              m->size, m->from, m->to);
    switch (m->routing_type) {
    case UNICAST: 
      if (!switchbox_locking_send(m) && debug)
        printf ("handle_connection: ERROR bad client %d (disconnected?).  \n", m->to);
      break;
    case BROADCAST: assert(broadcast(m)); break; }
      break;
    case MULTICAST:
      break;
    case ADMIN:
      switchbox_handle_admin(m); 
      break;
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
  for (int i = 0; i < MAX_GROUPS; i++){
     multicast_groups[i].members = new_dllist();
     multicast_groups[i].active  = false;
  }
  while (valid_socket(s = accept_connection(l))) setup_connection(s); }


void send_hello_message(Socket s, int addr){
    SBMessage * msg = malloc(sizeof(int)*4);
    msg->size = sizeof(int)*4;
    msg->routing_type = ADMIN;
    msg->from = addr;
    msg->to   = addr;
    switchbox_locking_send(msg);
}
