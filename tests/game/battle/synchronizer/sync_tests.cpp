#include "Synchronizer.hpp"
#include "Connection.hpp"
#include <cstdio>

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
char buf[512];

int main(){
    int connections;
    scanf("%d", &connections);
    //cout << connections << endl;
    for (int i = 0; i < connections; i++){
        cons.push_back(new Connection("localhost", SWITCHBOX_PORT));
        cons.at(i)->start();
    }
    //Connection mycon("localhost", SWITCHBOX_PORT);
    //mycon.start();
    usleep(10000); 

    CatInterpreter sint;
    CMBSynchronizer sync(cons.at(0), &sint);
    sync.Start();

    // Send a Null message.
    for (int i = 0; i < connections; i++){
        cons.at(i)->sendMessage(4*sizeof(int)+3, UNICAST, cons.at(0)->getAddress(), (char*)"0 ");
    }
    usleep(10000);
    sync.startSendToInt();

    while(1){
        int clientnum;
        if ( 1 != scanf("%d ", &clientnum) )
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
