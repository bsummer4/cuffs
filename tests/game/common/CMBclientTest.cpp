#include "Generator.hpp"
#include "Synchronizer.hpp"
#include "Interpreter.hpp"
#include "Connection.hpp"
#include <string>
using namespace std;

static const int SWITCHBOX_PORT = 5151;

//vector<Connection*> cons;
vector<string> hosts;
char buf[512];

int main() {
  int connections;
  string host, server;
  int i;

  cout << "Who's the server? (name only/eecs domain)" << endl;
  cin >> server; //better replacement
  server.append(".eecs.utk.edu");
  Connection *con = new Connection(server.c_str(), SWITCHBOX_PORT);
  con->start();

  //setup
  usleep(10000);
  CatInterpreter sint;
  CMBSynchronizer sync(con, &sint);
  Generator gen = Generator();
  gen.debug = false;
  srand(time(NULL));

  // Send a Null message. from
  con->sendMessage(4*sizeof(int)+3, BROADCAST, con->getAddress(), (char*)"0 "); //sent to switchbox?
  usleep(10000);
  sync.startSendToInt();

  for(i=0; i<10; i++) {
    gen.genStateMsg(buf, strlen(buf), sync.currentTime());
    con->sendMessage(4*sizeof(int)+strlen(buf)+1, BROADCAST, con->getAddress(), buf);
    usleep(1000);
  }
  sleep(2);
}
