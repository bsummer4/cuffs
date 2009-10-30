#include "Generator.hpp"
#include "Synchronizer.hpp"
#include "Interpreter.hpp"
#include "Connection.hpp"
#include <cstdio>
#include <string>
using namespace std;

vector<Connection*> cons;
vector<string> hosts;
char buf[512];

int main(){
    int connections;
    string host, server;
    int i;

    cout << "Who's the server? (name only/eecs domain)" << endl;
    cin >> server;
    server.append(".eecs.utk.edu");
 //  server = "hydra3.eecs.utk.edu";

  //  cout << "How many hosts?(up to 3)" << endl;
  //  scanf("%d", &connections);
  //  cout << "Please list  hosts on the eecs domain (name only: hydra3, cetus4, etc)" << endl;
  /*  for(i=0; i < connections; i++){
      //scanf("%s", &host); 
      cin >> host;
      hosts.push_back(host.append(".eecs.utk.edu"));
    }
    */
    //cout << connections << endl;
    //Push hosts on the hosts vector"
    cons.push_back(new Connection(server.c_str(), SWITCHBOX_PORT)); 
    cons.at(0)->start();
    //cons.push_back(new Connection("localhost", SWITCHBOX_PORT)); 

//    for (i = 0; i < 2; i++){
//      cons.push_back(new Connection(hosts[i].c_str(), SWITCHBOX_PORT));
//      cons.at(i)->start();
//    }

    //Connection mycon("localhost", SWITCHBOX_PORT);
    //mycon.start();
    usleep(10000); 
    CatInterpreter sint;
    CMBSynchronizer sync(cons.at(0), &sint);
    Generator gen = Generator();
    gen.debug = false;
    srand(time (NULL));

    // Send a Null message.
 //   for (int i = 0; i < connections; i++){
    cons.at(5)->sendMessage(4*sizeof(int)+3, BROADCAST, cons.at(0)->getAddress(), (char*)"0 "); //sent to switchbox?
 //   }
    sync.Start();

    //while(1){
    for(i=0;i<10;i++){
//      int clientnum;
//      if ( 1 != scanf("%d ", &clientnum) )
//        break;
      gen.genStateMsg(buf, strlen(buf), sync.currentTime());
      
/*      fgets(buf, 511, stdin);
      buf[strlen(buf)-1] = '\0';
      //cout << " clientnum = " << clientnum << " message: " << buf << endl;
*/
      cons.at(0)->sendMessage(4*sizeof(int)+strlen(buf)+1, BROADCAST, cons.at(0)->getAddress(), buf);
      usleep(1000);
      sleep(rand()%4+1);
    }
    sleep(2);
  //}
}
