#include "Connection.h"
#include "SwitchboxAdmin.h"
#include "SBTestCommon.h"
#include <iostream>

using namespace std;

const char* HOST="localhost";
char* test_message = "hello";

bool e1(Connection* c);
bool e2_1(SwitchboxAdmin* swa);
bool e2_2(SwitchboxAdmin* swa);

int main(){
    SwitchboxAdmin * swa = new SwitchboxAdmin(HOST, SWITCHBOX_PORT);
    swa->start();

    usleep(5000);

    bool passed = true;
    bool testPassed = false;

    testPassed = e1(swa);
    if ( passed  && !testPassed) passed = false;
    

    e2_1(swa);
    if ( passed  && !testPassed) passed = false;
    e2_2(swa);
    if ( passed  && !testPassed) passed = false;
    
    return passed;
}

bool e1(Connection* c){
    cout << "[EC1: Invalid Sent Destination] ";

    string errormsg;
    bool passed;
    int myaddr = c->getAddress();
    int badaddr = myaddr+1;

    int size = 1+strlen(test_message)+4*sizeof(int);
    message_type type = UNICAST;
    int from = myaddr;
    int to   = badaddr;
    c->sendMessage(size, type, to, test_message);
    usleep(10000);
    if ( c->getMessageCount() == 1 ){
       SBMessage* msg = c->getMessage(); 
       // Check that this is indeed the correct error message.
       bool testResults = SBTestCommon::TestMessage(msg, 4, INVALID_TARGET, 0, myaddr, NULL, errormsg);
       if ( testResults ) {
           cout << "passed" << endl;
       } else {
           cout << "failed: " << errormsg << endl;
       }
    } else {
        cout << "failed: bad number of message, got " << c->getMessageCount() << " expected 1" << endl;
        c->clearMessageQueue();
        return false;
    }
}

bool e2_1(SwitchboxAdmin* swa){
    cout << "[e21: Sending message to empty group] ";
    swa->createGroup(1);
    bool passed;
    int myaddr = swa->getAddress();
    int badaddr = myaddr+1;
    int size = 1+strlen(test_message)+4*sizeof(int);
    message_type type = MULTICAST;
    int from = myaddr;
    int to = 1;

    swa->sendMessage(size, type, to, test_message);
    usleep(10000);

    // Shouldn't get any errors
    if ( swa->getMessageCount() == 0 ){
        cout << " passed" << endl;
        return true;
    }
    else { 
        cout << " failed" << endl;
        swa->clearMessageQueue();
        return false;
    }
}

bool e2_2(SwitchboxAdmin* swa){
    cout << "[e22: Sending message to non-existant group] ";
    bool passed;
    int myaddr = swa->getAddress();
    int badaddr = myaddr+1;
    int size = 1+strlen(test_message)+4*sizeof(int);
    message_type type = MULTICAST;
    int from = myaddr;
    int to = 2;

    swa->sendMessage(size, type, to, test_message);
    usleep(10000);

    // Should get an error that the group doesn't exist
    if ( swa->getMessageCount() == 1 ){
        SBMessage* msg = swa->getMessage();
        std::string errormsg;
        bool testResults = SBTestCommon::TestMessage(msg, 4, INVALID_TARGET, 0, myaddr, NULL, errormsg);
        free(msg);
        if ( testResults ) {
            cout << "passed" << endl;
            return true;
        }
        else {
            cout << "failed: " << errormsg << endl;
            return false;
        }
    }
    else { 
        cout << "failed: Expected 1 message but got " << swa->getMessageCount() << endl;
        swa->clearMessageQueue();
        return false;
    }
}