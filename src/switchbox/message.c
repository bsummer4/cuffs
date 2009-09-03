#include "message.h"
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <assert.h>
#include <pthread.h>

#define TEST_STRING "hi"
#define TEST_PORT 80000

// # Testing
// The testing in this file basically just tests that the code
// actually does something and supplies a incredibly simple example of
// usage.  They do not attempt to make sure the code completly works.

void test_send_string(Socket socket);
void test_connections();

// The test routine for everything in this file
void test_message_c() {

  // * TODO "__test_message_c_.tmp" is bad form.
  //   I beleve the "correct" way to do this is something like:
  //   "/temp/$(process_id)_001/"

  // First we test the socket routines on a normal files
  Socket f = fopen("__test_message_c_.tmp", "w+");
  assert(f);
  test_send_string(f);
  fseek(f, 0, SEEK_SET);

  int length;
  char *result;
  read_array(f, &length, &result);

  // TODO this is unsafe: result may not be terminated.
  assert(streq(result, TEST_STRING));

  // Junk at the end.
  assert(!fread(result, 1, 1, f));

  // cleanup
  system("rm __test_message_c_.tmp");
  free(result);

  test_connections();
}


void *listen_send_then_die(void* arg) {
  Listener l = *(Listener*) arg;
  Socket s = accept_connection(l);

  int length;
  byte *result;
  read_array(s, &length, &result);
  iter(ii, 0, length - 4) assert(result[ii] == TEST_STRING[ii]); 

  free(result);
  test_send_string(s);

  pthread_exit(NULL);
}

// tests open_connection, make_listener, accept_connection like so:
// - Setup two threads: client and server
// - Send data one way... read it
// - Send data the other way... read it
// - yay
void test_connections() {
  Listener l = make_listener(TEST_PORT);
  assert(l != -1);
  pthread_t thread;
  pthread_create(&thread, NULL, listen_send_then_die, &l);
  Socket s = open_connection("localhost", TEST_PORT);
  assert(s);
  test_send_string(s);
  int length;
  byte *result;
  read_array(s, &length, &result);
  iter(ii, 0, length - 4) assert(result[ii] == TEST_STRING[ii]);
  
  void *ignore; pthread_join(thread, &ignore);
}

void test_send_string(Socket socket) {
  assert(send_string(socket, TEST_STRING)); }



// # Code
bool streq(char *s1, char *s2) { return !strcmp(s1, s2); }

// Sets results to length and array.
bool read_array(Socket socket, int *length, byte **array) {
  if (1 != fread(length, sizeof(int), 1, socket)) goto read_array_error;
  int array_size = *length - sizeof(int);
  *array = malloc(array_size);
  if (array_size != fread(*array, sizeof(byte), array_size, socket))
    goto read_array_error;
  return true;
 read_array_error: return false;
}


bool send_string(Socket socket, char* string) {
  return send_array(socket, sizeof(int) + strlen(string), string); }

/** Length is 4 + len(array).  I.E.  length is the size of the array
    plus the size of length itself.  */
bool send_array(Socket socket, int length, byte* array) {
  int array_length = length - 4;
  if ((fwrite(&length, sizeof(int), 1, socket) != 1) ||
      (fwrite(array, sizeof(byte), array_length, socket) != array_length))
    { perror("fwrite"); return false; }
  fflush(socket);
  return true;
}

/** Returns a NULL on error.  */
Socket open_connection(const char* hostname, const int port) {
  struct hostent *host = gethostbyname(hostname);
  if (!host) { perror("gethostbyname"); goto open_connection_error; }

  struct sockaddr_in host_connection;
  bzero(&host_connection, sizeof(host_connection));

  host_connection.sin_family = AF_INET;
  host_connection.sin_addr.s_addr = ((struct in_addr *)(host->h_addr))->s_addr;
  host_connection.sin_port = htons(port);

  int sd = socket(AF_INET, SOCK_STREAM, 0);
  if (sd == -1) { perror("socket"); goto open_connection_error; }

  if (connect(sd, (struct sockaddr *)  &host_connection,
              sizeof(host_connection)) == -1) {
    perror("connect"); goto open_connection_error; }

  FILE *result = fdopen(sd, "r+");
  return result;

 open_connection_error: return NULL;
}


// Returns -1 on error
Listener make_listener(int port) {
  Listener result;
  if ((result = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket"); goto make_listener_error; }

  struct sockaddr_in address;
  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  /* Bind the socket to the port number.  */
  if (bind(result, (struct sockaddr *) &address, sizeof(address)) == -1) {
    perror("bind"); goto make_listener_error; }

  /* Show that we are willing to listen.  */
  if (listen(result, 5) == -1) {
    perror("listen"); goto make_listener_error; }

  return result;

 make_listener_error: return -1;
}


// Returns NULL on error
Socket accept_connection(Listener l) {
  struct sockaddr_in address;
  int descriptor, addrlen = sizeof(address);
  
  if ((descriptor = accept(l, (struct sockaddr *) &address, &addrlen)) == -1) {
    perror("accept"); goto accept_connection_error; }

  FILE* result = fdopen(descriptor, "r+");
  if (!result) { perror("fdopen"); goto accept_connection_error; }
  return result;

 accept_connection_error: return NULL;
}
