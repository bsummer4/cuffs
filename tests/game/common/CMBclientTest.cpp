/*CMB clientTest is run on the client machines and will connect to the
  switchbox and print out*/
#include "Generator.hpp"
#include "Synchronizer.hpp"
#include "Interpreter.hpp"
#include "Connection.hpp"
#include <string>
using namespace std;

static const int SWITCHBOX_PORT = 5151;

vector<string> hosts;
char buf[512];

int main() {
  int connections;
  string host, server;
  int i;

  cout << "What is the server (hostname only)" << endl;
  cin >> server; //needs a better replacement
  server.append(".eecs.utk.edu");
  Connection *con = new Connection(server.c_str(), SWITCHBOX_PORT);
  con->start();
  if (!con->isRunning()) return 0;

  //setup
  usleep(10000);
  CatInterpreter sint;
  CMBSynchronizer sync(con, &sint);
  Generator gen = Generator();
  gen.debug = false;
  srand(time(NULL));

  // Send a Null message. from
  con->sendMessage(4*sizeof(int)+3, BROADCAST, con->getAddress(), "0 "); //sent to switchbox?
  usleep(10000);
  sync.startSendToInt();

  for (i=0; i<10; i++) {
    gen.genStateMsg(buf, 512, sync.currentTime());
    con->sendMessage(4*sizeof(int)+strlen(buf)+1, BROADCAST, con->getAddress(), buf);
    usleep(1000);
  }
  sleep(2);
  return (1);
}
