#include <stdio.h>
#include <cstring>
#include <iostream>
#include "Connection.h"
#include "SwitchboxAdmin.h"
#include "SBTestCommon.h"

#include <unistd.h>

using namespace std;

// Prototypes for functions for tests below See doc for more details.
// F1
int f1(Connection* c, const char* testName);
bool f2(Connection * send, Connection* receive, Connection* none, const char* testName);
bool f2_2(Connection * send, Connection* receive, Connection* n1, Connection* n2, const char* testName);
bool f3(Connection * send, Connection* r1, Connection* r2, const char* testName);
bool f3_2(Connection * send, Connection* r1, Connection* r2, Connection* r3, const char* testName);
bool f4(Connection* c1, Connection* c2, Connection* c3, bool r1, bool r2, bool r3, int groupNum, const char* testName);

int main(int argc, char* argv[]){

    SwitchboxAdmin * swa = new SwitchboxAdmin(HOST, SWITCHBOX_PORT);
    swa->start();
    Connection * clientA = new Connection(HOST, SWITCHBOX_PORT);
    Connection * clientB = new Connection(HOST, SWITCHBOX_PORT);
    Connection * clientC = new Connection(HOST, SWITCHBOX_PORT);
    Connection * clientD = new Connection(HOST, SWITCHBOX_PORT);

    ////////////////////////////////////////////////////////////////////////////////
    // Test 1 - F1:  Logon – join
    ////////////////////////////////////////////////////////////////////////////////
    cout << "[F1: Logon - join]" << endl;
    int addressA = f1(clientA, "F1a:");
    int addressB = f1(clientB, "F1b:");
    int addressC = f1(clientC, "F1c:");

    if ( addressA == -1 ||
         addressB == -1 ||
         addressC == -1){
        cout << "Exiting. Can not continue if logon tests fail." << endl;
        exit(1);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // Test 2 - F2: Point-To-Point Communication
    ////////////////////////////////////////////////////////////////////////////////
    cout << "[F2: Point-To-Point Communication]" << endl;
    if ( !f2(clientA, clientB, clientC, "F2a:") ){
        //exit(2);
    }
    if ( !f2(clientB, clientC, clientA, "F2b:") ){
        //exit(2);
    }
    if ( !f2(clientC, clientA, clientB, "F2c:") ){
        //exit(2);
    }
    // Start up clientD: 
    clientD->start();
    usleep(USLEEP_TIME);
    if ( !f2_2(clientA, clientD, clientB, clientC, "F2d:") ){
        //exit(2);
    }
    // Disconnect clientD
    cout << "clientd stop" << endl;
    clientD->stop();
    cout << "after clientd stop" << endl;

    // TODO: Fix this, you should be able to stop and restart
    delete clientD;
    clientD = new Connection(HOST, SWITCHBOX_PORT);

    ////////////////////////////////////////////////////////////////////////////////
    // Test 3 - Collective Communication: Broadcast
    ////////////////////////////////////////////////////////////////////////////////
    cout << "[F3: Collective Communication: Broadcast]" << endl;
    if ( !f3(clientA, clientB, clientC, "F3a:") ) {
        //exit(3);
    }
    if ( !f3(clientB, clientC, clientA, "F3b:") ){
        //exit(3);
    }
    if ( !f3(clientC, clientA, clientB, "F3c:") ){
        //exit(3);
    }
    clientD->start();
    usleep(USLEEP_TIME);
    if ( !f3_2(clientA, clientB, clientC, clientD, "F3d:") ){
        //exit(3);
    }
    cout << "clientd stop" << endl;
    clientD->stop();
    cout << "after clientd stop" << endl;

    ////////////////////////////////////////////////////////////////////////////////
    // Test 4 - F4: Collective communication: Group-based multicast
    ////////////////////////////////////////////////////////////////////////////////
    cout << "[F4: Collective communication: Group-based multicast]" << endl;
    //swa->createGroup(1);
    swa->def_group(1, &addressB, 1);
    if ( !f4(clientA, clientB, clientC, false, true, false, 1, "F4a:") )
        exit(4);
    int address[2];
    address[0] = addressB;
    address[1] = addressC;
    swa->def_group(1, &address, 2);
    if ( !f4(clientA, clientB, clientC, false, true, true, 1, "F4b:") )
        exit(4);
    
    swa->def_group(1, &addressB, 1);
    if ( !f4(clientA, clientB, clientC, false, true, false, 1, "F4c:") )
        exit(4);

    swa->stop();
    return 0;
}

int f1(Connection* c, const char* testName){
    int retval;
    cout << "[" << testName << "] ";
    c->start();
    usleep(USLEEP_TIME);
    retval = c->getAddress();
    if (retval > -1){
        cout << "passed: address is " << retval << endl;
    } else {
        cout << "failed" << endl;
        //exit(1);
    }

    c->clearMessageQueue();

    return retval;
}

bool f2(Connection * send, Connection* receive, Connection* none, const char* testName){
    cout << "[" << testName << "] ";
    bool passed = false;
    int size = 1+strlen(test_message)+4*sizeof(int);
    message_type type = UNICAST;
    int from = send->getAddress();
    int to   = receive->getAddress();
    send->sendMessage(size, type, to, test_message);
    usleep(10000);
    if ( receive->getMessageCount() == 1 && none->getMessageCount() == 0 ){
        SBMessage* msg = receive->getMessage();
        if ( msg->size == size &&
             msg->to   == to   &&
             msg->from == from &&
             msg->routing_type == type && 
             strcmp(msg->data, test_message) == 0){
            passed = true;
        }
        else {
            cout << "here" << endl;
        }
        free(msg);
    }

    // Now clear all message queues for next test
    send->clearMessageQueue();
    receive->clearMessageQueue();
    none->clearMessageQueue();

    if (passed)
        cout << "passed" << endl;
    else
        cout << "failed" << endl;

    return passed;
}

bool f2_2(Connection * send, Connection* receive, Connection* n1, Connection* n2, const char* testName){
    cout << "[" << testName << "] ";
    bool passed = false;
    int size = 1+strlen(test_message)+4*sizeof(int);
    message_type type = UNICAST;
    int from = send->getAddress();
    int to   = receive->getAddress();
    send->sendMessage(size, type, to, test_message);
    usleep(USLEEP_TIME);
    if ( receive->getMessageCount() == 1 && n1->getMessageCount() == 0 && n2->getMessageCount() == 0 && send->getMessageCount() == 0){
        SBMessage* msg = receive->getMessage();
        if ( msg->size == size &&
             msg->to   == to   &&
             msg->from == from &&
             msg->routing_type == type && 
             strcmp(msg->data, test_message) == 0){
            passed = true;
        }
        free(msg);
    }

    // Now clear all message queues for next test
    send->clearMessageQueue();
    receive->clearMessageQueue();
    n1->clearMessageQueue();
    n2->clearMessageQueue();

    if (passed)
        cout << "passed" << endl;
    else
        cout << "failed" << endl;

    return passed;
}


bool f3(Connection * send, Connection* r1, Connection* r2, const char* testName){
    cout << "[" << testName << "] ";
    bool passed = false;
    int size = 1+strlen(test_message)+4*sizeof(int);
    message_type type = BROADCAST;
    int from = send->getAddress();
    int to   = -1;
    Connection * clist[3];
    clist[0] = send;
    clist[1] = r1;
    clist[2] = r2;
    send->sendMessage(size, type, to, test_message);
    usleep(USLEEP_TIME);
    //cout << send->getMessageCount() << endl;
    //cout << r1->getMessageCount() << endl;
    //cout << r2->getMessageCount() << endl;
    if ( send->getMessageCount() == 1 &&
           r1->getMessageCount() == 1 &&
           r2->getMessageCount() == 1){
        for (int i = 0; i < 3; i++){
            SBMessage* msg = clist[i]->getMessage();
            if ( msg->size == size &&
                 msg->from == from &&
                 msg->routing_type == type && 
                 strcmp(msg->data, test_message) == 0){
                passed = true;
            }
        }
    }

    if (passed)
        cout << "passed" << endl;
    else
        cout << "failed" << endl;

    return passed;
}

bool f3_2(Connection * send, Connection* r1, Connection* r2, Connection* r3, const char* testName){
    cout << "[" << testName << "] ";
    bool passed = false;
    int size = 1+strlen(test_message)+4*sizeof(int);
    message_type type = BROADCAST;
    int from = send->getAddress();
    int to   = -1;
    Connection * clist[4];
    clist[0] = send;
    clist[1] = r1;
    clist[2] = r2;
    clist[3] = r3;
    send->sendMessage(size, type, to, test_message);
    usleep(USLEEP_TIME);
    if ( send->getMessageCount() == 1 &&
           r1->getMessageCount() == 1 &&
           r2->getMessageCount() == 1 &&
           r3->getMessageCount() == 1){
        for (int i = 0; i < 4; i++){
            SBMessage* msg = clist[i]->getMessage();
            if ( msg->size == size &&
                 msg->from == from &&
                 msg->routing_type == type && 
                 strcmp(msg->data, test_message) == 0){
                passed = true;
            }
        }
    }

    if (passed)
        cout << "passed" << endl;
    else
        cout << "failed" << endl;

    return passed;
}

bool f4(Connection* c1, Connection* c2, Connection* c3, bool r1, bool r2, bool r3, int groupNum, const char* testName){
    cout << "[" << testName << "] ";
    int size = 1+strlen(test_message)+4*sizeof(int);
    message_type type = MULTICAST;
    int from = c1->getAddress();
    int to   = groupNum;
    Connection * clist[4];
    clist[0] = c1;
    clist[1] = c2;
    clist[2] = c3;
    bool blist[3];
    blist[0] = r1;
    blist[1] = r2;
    blist[2] = r3;

    c1->sendMessage(size, type, to, test_message);
    usleep(USLEEP_TIME);

    for (int i = 0; i < 3; i++){
        if ( blist[i] && clist[i]->getMessageCount() == 1 ){
            string errormsg;
            SBMessage * msg = clist[i]->getMessage();
            if ( !SBTestCommon::TestMessage(msg, size, MULTICAST, from, to, test_message, errormsg) ){
                cout << "failed: " << errormsg << endl;
                return false;
            }
        }
        else if ( !blist[i] && clist[i]->getMessageCount() != 0){
            cout << "failed: Got message and wasn't supposed to" << endl;
            return false;
        }
        else { 
            cout << "failed: Got no message, but I was supposed to" << endl;
            return false;
        }
    }

    cout << "passed" << endl;
    return true;
}
