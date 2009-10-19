#include "Synchronizer.hpp"
#include "Connection.hpp"
#include <iostream>
#include "switchbox_client.h"
/**
 * @defgroup SyncTest Testing
 * @ingroup Synchronizer
 */
/**
 * @addtogroup SyncTest
 * @{
 * @addtogroup SimpleSynchronizer_Test
 * @{
 */
/**
 * @file 
 * @author John R. Hoare
 *
 * Tests the SimpleSynchronizer Class
 */

using namespace std;

vector<Connection*> cons;
char buf[512];

int main(){
    int connections;
    scanf("%d", &connections);
    for (int i = 0; i < connections; i++){
        cons.push_back(new Connection("localhost", SWITCHBOX_PORT));
        cons.at(i)->start();
    }
    Connection mycon("localhost", SWITCHBOX_PORT);
    mycon.start();
    usleep(10000); 

    CatInterpreter sint;
    SimpleSynchronizer sync(&mycon, &sint);
    // Send a Null message.
    for (int i = 0; i < connections; i++){
        cons.at(i)->sendMessage(4*sizeof(int)+3, UNICAST, mycon.getAddress(), "0 ");
    }
    sync.Start();

    while(1){
        int clientnum;
        if ( 1 != scanf("%d ", &clientnum) )
            break;
        fgets(buf, 511, stdin);
        buf[strlen(buf)-1] = '\0';
        cons.at(clientnum)->sendMessage(4*sizeof(int)+strlen(buf)+1, UNICAST, mycon.getAddress(), buf);
    }

    sleep(2);
    mycon.stop();
}

/**
 * @}
 * @}
 */
