#include "message.h"
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <assert.h>

#define TEST_STRING "hi"

void test_send_string(Socket socket);

// # Testing
// The testing in this file basically just tests that the code
// actually does something and supplies a incredibly simple example of
// usage.  They do not attempt to make sure the code completly works.

// The test routine for everything in this file
void test_message_c() {

  // First we test the socket routines on a file.
  Socket f = fopen("__test_message_c_.tmp", "w+");
  assert(f);
  test_send_string(f);
  fseek(f, 0, SEEK_SET);

  int length;
  char *result;
  read_array(f, &length, &result);
  assert(streq(result, TEST_STRING));

  // There's no junk at the end.
  assert(!fread(result, 1, 1, f));

  // cleanup
  system("rm __test_message_c_.tmp");
  free(result);
}

// We need to have something listening before we can open a socket.
void test_open_socket() {
  // TODO write this
  // I think we basically need to:
  // - Make a listener
  // - Start a thread to listen which cleans up when the thread dies.
  // - Connect
  // - close the connection
  // - cleanup
  assert(false);
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
Socket open_socket(const char* hostname, const int port) {
  struct hostent *host = gethostbyname(hostname);
  if (!host) { perror("gethostbyname"); goto open_socket_error; }

  struct sockaddr_in host_connection;
  bzero(&host_connection, sizeof(host_connection));

  host_connection.sin_family = AF_INET;
  host_connection.sin_addr.s_addr = ((struct in_addr *)(host->h_addr))->s_addr;
  host_connection.sin_port = htons(port);

  int sd = socket(AF_INET, SOCK_STREAM, 0);
  if (sd == -1) { perror("socket"); goto open_socket_error; }

  if (connect(sd, (struct sockaddr *)  &host_connection,
              sizeof(host_connection)) == -1) {
    perror("connect"); goto open_socket_error; }

  FILE *result = fdopen(sd, "r+");

 open_socket_error:
  return NULL;
}
