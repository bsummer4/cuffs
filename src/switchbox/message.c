#include "message.h"
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>

#define TEST_STRING "hi"
#define TEST_PORT 80009

bool debug = false;

// =======
// Testing
// =======
//
// The testing in this file basically just tests that the code
// actually does something and supplies a incredibly simple example of
// usage.  They do not attempt to make sure the code completly works.

void test_send_string(Socket socket);
void test_connections();
void *listen_send_then_die(void *arg);
void test_send(Socket s);
void test_recieve(Socket s);
void test_(Socket s);

// tests open_connection, make_listener, accept_connection like so:
// - Setup two threads: client and server
// - Send data one way... read it
// - Send data the other way... read it
// - yay
void test_message_c() {
  Listener l = make_listener(TEST_PORT);
  assert(l != -1);
  pthread_t thread;
  pthread_create(&thread, NULL, listen_send_then_die, &l);
  Socket s = open_connection("localhost", TEST_PORT);
  assert(valid_socket(s));
  test_send(s);
  test_recieve(s);
  void *ignore;
  pthread_join(thread, &ignore);
}

void test_send(Socket s) {
  assert(send_string(s, TEST_STRING));
}

void test_recieve(Socket s) {
  Message *m = receive_message(s);
  char *string = m->data;
  iter(ii, 0, m->size - sizeof(int)) assert(string[ii] == TEST_STRING[ii]);
  free(m);
}

void *listen_send_then_die(void *arg) {
  Listener l = *(Listener *) arg;
  Socket s = accept_connection(l);
  test_recieve(s);
  test_send(s);
  pthread_exit(NULL);
}



// ==============
// Implementation
// ==============

bool streq(char *s1, char *s2) {
  return !strcmp(s1, s2);
}


// --------
// Messages
// --------

bool send_message(Socket socket, Message * m) {
  if (debug)
    printf("send_message:  It's going out\n");
  bool success = write_bytes((byte *) m, m->size, socket);
  if (debug)
    printf("send_message:  %s.\n", (success ? "Success!" : "FAILURE!!!"));
  return success;
}

// Returns NULL if the socket is id or closed.
Message *receive_message(Socket socket) {
  Message *result = NULL;
  int size;
  if (!valid_socket(socket))
    goto receive_message_error;
  bool success = read_bytes((byte *) & size, sizeof(int), socket);
  if (!success)
    goto receive_message_error;
  result = malloc(size + 1);    // need space for null at the end
  if (!result)
    goto receive_message_error;
  result->size = size;
  size_t size_remaining = size - sizeof(int);
  success = read_bytes(result->data, size_remaining, socket);
  if (!success)
    goto receive_message_error;
  char *result_c = (char *) result;
  result_c[result->size] = '\0';
  return result;
receive_message_error:
  if (result)
    free(result);
  return NULL;
}

bool send_string(Socket socket, char *string) {
  return send_array(socket, sizeof(int) + strlen(string), string);
}


/** This is basically the same as send_message().  However, the data
    you want to send wont always be laid out that way.

    'length' should be the size of 'length' + the size of 'array' (4 +
    len(array)).  */
bool send_array(Socket socket, int length, byte * array) {
  size_t array_length = length - sizeof(int);
  return (write_bytes((byte *) & length, sizeof(int), socket) &&
          write_bytes(array, array_length, socket));
}



// -----------
// Connections
// -----------

bool write_bytes(byte * output, size_t length, Socket s) {
  char *pointer = output;
  while (pointer < output + length) {
    int written = write(s, pointer, output - pointer + length);
    if (written < 0)
      return false;
    pointer += written;
  }
  return true;
}

bool read_bytes(byte * input, size_t length, Socket s) {
  if (!valid_socket(s))
    return false;
  char *pointer = input;
  while (pointer < input + length) {
    int bytes_read = read(s, pointer, input - pointer + length);
    if (bytes_read == 0)
      return false;
    if (bytes_read < 0) {
      if (debug)
        perror("read");
      return false;
    }
    pointer += bytes_read;
  }
  return true;
}

bool valid_socket(Socket socket) {
  return socket != -1;
}

bool valid_listener(Listener listener) {
  return listener != -1;
}

/** Returns an invalid socket on error.  */
Socket open_connection(const char *hostname, const int port) {
  struct hostent *host = gethostbyname(hostname);
  if (!host) {
    perror("gethostbyname");
    goto open_connection_error;
  }

  struct sockaddr_in connection;
  bzero(&connection, sizeof(connection));

  connection.sin_family = AF_INET;
  connection.sin_addr.s_addr = ((struct in_addr *) (host->h_addr))->s_addr;
  connection.sin_port = htons(port);

  int descriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (descriptor == -1) {
    perror("socket");
    goto open_connection_error;
  }

  if (connect(descriptor,
              (struct sockaddr *) &connection, sizeof(connection)) == -1) {
    perror("connect");
    goto open_connection_error;
  }
  return descriptor;

open_connection_error:return -1;
}


// Returns an invalid listener on error
Listener make_listener(int port) {
  Listener result;
  if ((result = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    goto make_listener_error;
  }

  struct sockaddr_in address;
  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  /* Bind the socket to the port number.  */
  if (bind(result, (struct sockaddr *) &address, sizeof(address)) == -1) {
    perror("bind");
    goto make_listener_error;
  }

  /* Show that we are willing to listen.  */
  if (listen(result, 5) == -1) {
    perror("listen");
    goto make_listener_error;
  }

  return result;

make_listener_error:return -1;
}


// Returns an invalid Socket on error
Socket accept_connection(Listener l) {
  struct sockaddr_in address;
  int descriptor, addrlen = sizeof(address);

  if ((descriptor = accept(l, (struct sockaddr *) &address, &addrlen)) == -1) {
    perror("accept");
    goto accept_connection_error;
  }

  return descriptor;

accept_connection_error:return -1;
}

void close_connection(Socket s) {
  shutdown(s, SHUT_RDWR);
  close(s);
}
