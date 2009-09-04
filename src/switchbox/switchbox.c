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
  // test_message_c();
  // fputs("All tests passed.  ", stdout);

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
  int id = *(int*)id_p;
  Socket s = open_connection("localhost", SWITCHBOX_PORT);
  int size = sizeof(SBMessage) + 3; // header + 'h' 'i' '\0'
  SBMessage *m = malloc(size);
  m->size = size;
  m->routing_type = UNICAST;
  m->from = id;
  m->to = id;
  strcpy(m->data, "hi");
  while (true) {
    printf("spammer #%d: Sending 'hi' to myself.  \n", id);
    switchbox_send(s, m);
    printf("spammer: Waiting for response.  \n");
    SBMessage *result = switchbox_receive(s);
    assert(result->size == m->size);
    iter(ii, 0, m->size) assert(((byte*)result)[ii] == ((byte*)m)[ii]);
    printf("spammer: got it!  sleeping...  \n");
    sleep(5);
  }

  return NULL;
}

void setup_connection(Socket s) {
  Client *c = get_new_client(s);
  printf("setup_connection: assigning client #%d.  \n", c-clients);
  pthread_create(&(c->thread), NULL, handle_connection, c); }

bool run_switchbox(int port) {
    Socket s;
    Listener l = make_listener(port);
    pthread_t spam;
    int a=0, b=1, c=2;
    pthread_create(&spam, NULL, spammer, &a);
    pthread_create(&spam, NULL, spammer, &b);
    pthread_create(&spam, NULL, spammer, &c);
    while (s = accept_connection(l)) setup_connection(s); }

bool switchbox_send(Socket s, SBMessage* m) {
  return send_message(s, (Message*) m); }

SBMessage *switchbox_receive(Socket s) {
  return (SBMessage*) receive_message(s); }
