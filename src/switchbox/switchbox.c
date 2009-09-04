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

int main (int argc, char **argv) {
  return (int) run_switchbox(SWITCHBOX_PORT);
}


// -----------------------------
// Clients and the clients array
// -----------------------------
typedef struct client {
  Socket connection; // this slot should never be NULL
  pthread_t thread;
} Client;

Client clients[MAX_CLIENTS];
int first_free_client= 0;

void update_free_client() {
  while (clients[first_free_client].connection) first_free_client++; }

Client *get_new_client(Socket s) {
  if (first_free_client >= MAX_CLIENTS) return NULL;
  Client *result = &clients[first_free_client++];
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
    assert(m->from == client_id);
    printf
      ("handle_connection: new message [%d bytes] #%d -> #%d.  \n",
       m->size, client_id, m->to);
    switchbox_send(clients[m->to].connection, m);
    free(m); }

  remove_client(client);
  pthread_exit(NULL); }

void *spammer(void *id_p) {
  int *ids = (int*)id_p;
  Socket s = open_connection("localhost", SWITCHBOX_PORT);

  int size = sizeof(SBMessage) + 3; // header + 'h' 'i' '\0'
  SBMessage *m = malloc(size);
  m->size = size;
  m->routing_type = UNICAST;
  m->from = ids[0];
  m->to = ids[1];
  int sending = ids[2];

  strcpy(m->data, "hi");
  iter(iteration, 0, 5) { 
    if (m->to != -1) {
      printf("spammer #%d: Sending 'hi' to #%d.  \n", m->from, m->to);
      switchbox_send(s, m);
    }
    if (sending) {
      printf("spammer: Waiting for response.  \n");
      SBMessage *result = switchbox_receive(s);
      assert(result->size == m->size);
      iter(ii, 0, m->size) assert(((byte*)result)[ii] == ((byte*)m)[ii]);
    }
    printf("spammer: yay!  sleeping...  \n");
    sleep(3);
  }

  pthread_exit(NULL);
}

void setup_connection(Socket s) {
  Client *c = get_new_client(s);
  printf("setup_connection: assigning client #%d.  \n", c-clients);
  pthread_create(&(c->thread), NULL, handle_connection, c); }

bool run_switchbox(int port) {
  Socket s;
  Listener l = make_listener(port);
  pthread_t p1, p2, p3;
  int a[3]={0, 0, 1}, b[3]={1, 1, 1}, c[3]={2, 2, 1};
  pthread_create(&spam, NULL, spammer, &a);
  pthread_create(&spam, NULL, spammer, &b);
  pthread_create(&spam, NULL, spammer, &c);
  iter(ii, 0, 3) setup_connection(accept_connection(l));
  void *v = NULL;
  pthread_join(p1, &v);
  pthread_join(p2, &v);
  pthread_join(p3, &v);
  return true;
}

bool switchbox_send(Socket s, SBMessage* m) {
  return send_message(s, (Message*) m); }

SBMessage *switchbox_receive(Socket s) {
  return (SBMessage*) receive_message(s); }
