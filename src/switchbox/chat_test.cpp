#include "Connection.hpp"
#include <iostream>
#include <string>

#include <pthread.h>

using namespace std;

char buf[1024];

void * receive_data(void* args);
void * send_data(void* args);
string name;

int main() {
  Connection * c = new Connection("localhost", 80021);
  cout << "What is your name? ";
  cin >> name;
  cout << "Hello, " << name << endl;
  name += ": ";

  pthread_t t1;
  pthread_t t2;

  c->start();
  pthread_create(&t1, NULL, send_data, (void*) c);
  pthread_create(&t2, NULL, receive_data, (void*) c);

  pthread_join(t1, NULL);
  pthread_cancel(t2);
  pthread_join(t2, NULL);

  c->stop();
}


void *send_data(void* args) {
  Connection * c = (Connection*) args;
  string line;
  while(1) {
    cin.getline(buf, 1024);
    if(cin.fail())
      pthread_exit(NULL);

    char *content = (char*)(name + buf).c_str();
    c->sendMessage(string_to_message(BROADCAST, c->getAddress(), -1,
                                     content));
  }
}

void *receive_data(void* args) {
  Connection * c = (Connection*) args;
  while(1) {
    c->blockForMessage();
    SBMessage* sm = c->getMessage();
    cout << sm->data << endl;
    free(sm);
  }
}
