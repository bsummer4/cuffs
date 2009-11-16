/*CMB singleTest will connect to itself as if it were
  multiple clients. First, start a switchbox on itself then running
  the CMB triad to the switchbox. Fially outputting the contents
  to a file*/

#include "Generator.hpp"
#include "Synchronizer.hpp"
#include "Interpreter.hpp"
#include "Connection.hpp"
#include <cstdio>
#include <string>
using namespace std;

static const int SWITCHBOX_PORT = 5151;

vector<Connection*> cons;
char buf[512];

int main() {
  int connections;
  int i;
 /* if(1 != scanf("%d ", &connections)) {
    cout << "error: bad file" << endl;
    exit(0);
  }
  */
  connections = 3;
  //cout << connections << endl;
  for(int i = 0; i < connections; i++) {
    cons.push_back(new Connection("localhost", SWITCHBOX_PORT));
    cons.at(i)->start();
  }
  //Connection mycon("localhost", SWITCHBOX_PORT);
  //mycon.start();
  usleep(10000);

  CatInterpreter sint;
  CMBSynchronizer sync(cons.at(0), &sint);
  Generator gen;
  gen.debug = false;
  sync.Start();
  srand(time(NULL));

  // Send a Null message.
  for(i = 0; i < connections; i++) {
    cons.at(i)->sendMessage(4*sizeof(int)+3, BROADCAST, cons.at(0)->getAddress(), (char*)"0 ");
  }
  usleep(10000);
  sync.startSendToInt();

  for(i=0; i<10; i++) {
    int clientnum;
    clientnum = (int)(rand() % connections);
    gen.genStateMsg(buf, 512, sync.currentTime());
    //cout << " clientnum = " << clientnum << " message: " << buf << endl;
    cons.at(clientnum)->sendMessage(4*sizeof(int)+strlen(buf)+1, BROADCAST, cons.at(0)->getAddress(), buf);
    usleep(1000);
    //sleep(1);
  }
  sleep(2);
  return 1;
}

