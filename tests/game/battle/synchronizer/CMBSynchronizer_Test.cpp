#include "Synchronizer.hpp"
#include "Connection.hpp"
#include <cstdio>
#include <cstring>

static const int SWITCHBOX_PORT = 5151;

/**
 * @addtogroup SyncTest
 * @{
 * @addtogroup CMBSynchronizer_Test
 * @{
 */
/**
 * @file
 * @author John R. Hoare
 *
 * Tests the CMBSynchronizer Class
 */

/**
 * @class CMBSynchronizer
 *
 * @test That given known imput that the output is correct and in order.
 */

using namespace std;

vector<Connection*> cons;
vector<int> clients;
char buf[512];

int main() {
  int connections;
  scanf("%d", &connections);
  cerr << "Connections: " << connections << endl;
  assert(connections>0);
  for(int i = 0; i < connections; i++) {
    cons.push_back(new Connection("localhost", SWITCHBOX_PORT));
    cons.at(i)->start();
  }
  //Connection mycon("localhost", SWITCHBOX_PORT);
  //mycon.start();
  //usleep(10000);
  sleep(1);

  // Get all the Addresses
  for(int i = 0; i < connections; i++) {
    assert(cons.at(i)->getAddress() != -1);
    clients.push_back(cons.at(i)->getAddress());
    //cons.at(i)->sendMessage(4*sizeof(int)+5, UNICAST, cons.at(0)->getAddress(), "0.0 ");
  }

  CatInterpreter sint;
  CMBSynchronizer sync(cons.at(0), &sint, clients);
  sync.Start();

  while(1) {
    int clientnum;
    if(1 != scanf("%d ", &clientnum))
      break;
    fgets(buf, 511, stdin);
    buf[strlen(buf)-1] = '\0';
    //cout << " clientnum = " << clientnum << " message: " << buf << endl;
    cons.at(clientnum)->sendMessage(4*sizeof(int)+strlen(buf)+1, UNICAST, cons.at(0)->getAddress(), buf);
    usleep(1000);
  }
  sleep(2);
}
/**
 * @}
 * @}
 */
