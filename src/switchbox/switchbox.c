#include "switchbox.h"
#include <pthread.h>
#include <assert.h>
#include <unistd.h>

#define SWITCHBOX_PORT 80009
#define MAX_CLIENTS 1024

bool run_switchbox();


// ==============
// Implementation
// ==============

bool debug = true;

int main (int argc, char **argv) {
  return (int) run_switchbox(SWITCHBOX_PORT);
}


// -----------------------------
// Clients and the clients array
// -----------------------------

typedef struct client {
  // The 'connection' slot should never be NULL.  If is NULL it will
  // be treated as if the whole struct is unused.
  Socket connection;
  pthread_t thread;
} Client;

// the 'clients' array stores all clients in the system.  The clients
// 'id' corresponds to their index in this array.
Client clients[MAX_CLIENTS];

// The index of the first unused slot in the 'clients' array.
int first_free_client= 0;

// Moves 'first_free_client forward until it points to a free slot
void update_free_client() {
  while (clients[first_free_client].connection) first_free_client++; }

// Returns NULL if there isn't room for another client.
Client *get_new_client(Socket s) {
  if (first_free_client >= MAX_CLIENTS) return NULL;
  Client *result = clients + first_free_client;
  result->connection = s;
  update_free_client();
  return result;
}


bool remove_client(Client *client) {
  // TODO if the socket or thread are bad or the client is null, then
  //      return false.
  close_connection(client->connection);
  bzero(client, sizeof(Client));
  int index = client - clients;

  // If were the new first client.  Move first_free_client
  if (first_free_client > index) first_free_client = index;
  return true;
}

// -----------
// Connections
// -----------

// TODO add locking for the clients
void *handle_connection(void *client_) {
  Client* client = client_;
  SBMessage *m;
  int client_id = client - clients;
  while (m = switchbox_receive(client->connection)) {
    m->from = client_id; // Client can't lie about its identity.
    if (debug)
      printf ("handle_connection: new message [%d bytes] #%d -> #%d.  \n",
              m->size, client_id, m->to);
    switchbox_send(clients[m->to].connection, m);
    free(m); }

  // The connection has been closed.
  remove_client(client);
  pthread_exit(NULL); }

void setup_connection(Socket s) {
  Client *c = get_new_client(s);
  if (debug) printf("setup_connection: assigning client #%d.  \n", c-clients);
  pthread_create(&(c->thread), NULL, handle_connection, c); }

bool run_switchbox(int port) {
  Socket s;
  Listener l = make_listener(port);
  while (m = accept_connection(l)) setup_connection(m);
  return true;
}

bool switchbox_send(Socket s, SBMessage* m) {
  return send_message(s, (Message*) m); }

SBMessage *switchbox_receive(Socket s) {
  return (SBMessage*) receive_message(s); }
