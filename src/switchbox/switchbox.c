#include "switchbox_client.h"
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>

#define SWITCHBOX_PORT 80021
#define MAX_CLIENTS 1024

void run_switchbox(int port);


// ==============
// Implementation
// ==============

bool debug = true;

int main (int argc, char **argv) {
  // A broken pipe should just cause read() and write() to fail.
  signal(SIGPIPE, SIG_IGN);
  run_switchbox(SWITCHBOX_PORT);
  return 0; }


// -----------------------------
// Clients and the clients array
// -----------------------------

typedef struct client {
  // The 'thread' slot should never be NULL.  If is NULL it will
  // be treated as if the whole struct is unused.
  Socket connection;
  pthread_t thread;
  pthread_mutex_t lock;
  bool used;
} Client;

// The 'clients' array stores all clients in the system.  The clients
// 'id' corresponds to their index in this array.
Client clients[MAX_CLIENTS];

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
  // TODO if the socket or thread are bad or the client is null, then
  //      return false.
  int index = client - clients;
  if (debug) printf("Lost connection from client %d\n", index);
  if (!is_client_used(client)) return false;
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

// Locks the target client until it finishes sending the message.
bool switchbox_locking_send(SBMessage* m) {
  Client *target = clients + m->to;
  Socket socket = target->connection;
  printf("switchbox_locking_send: Locking before sending message to %d.  \n",
         m->to);
  pthread_mutex_lock(&target->lock);
  bool success = switchbox_send(socket, m);
  printf("switchbox_locking_send: [%d] Sent, unlocking... \n",
         m->to);
  pthread_mutex_unlock(&target->lock);
  return success; }

void *handle_connection(void *client_) {
  Client* client = client_;
  SBMessage *m;
  int client_id = client - clients;
  while ((m = switchbox_receive(client->connection))) {
    m->from = client_id; // Client can't lie about its identity.
    if (debug)
      printf ("handle_connection: new message [%s][%d bytes] #%d -> #%d.  \n",
              (m->routing_type ? "broadcast" : "unicast"),
              m->size, m->from, m->to);

    switch (m->routing_type) {
    case UNICAST:
      if (is_client_used(clients + m->to)) switchbox_locking_send(m);
      else if (debug)
        printf ("handle_connection: ERROR %d is not connected.  \n", m->to);
      break;
    case BROADCAST:
      iter(ii, 0, last_used_client+1) {
          if (debug) printf("broadcasting to %d\n", ii);
        if (is_client_used(clients + ii)) {
          m->to = ii;
          switchbox_locking_send(m); }}}
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
  while (valid_socket(s = accept_connection(l))) setup_connection(s); }
