#include "Connection.h"
#include "SwitchboxAdmin.h"
#include "SBTestCommon.h"
#include <iostream>

using namespace std;

bool e1(Connection* c);
bool e2_1(SwitchboxAdmin* swa);
bool e2_2(SwitchboxAdmin* swa);

int main(){
    SwitchboxAdmin * swa = new SwitchboxAdmin(HOST, SWITCHBOX_PORT);
    swa->start();
    usleep(USLEEP_TIME);

    //cout << "swa address: " << swa->getAddress() << endl;

    bool passed = true;
    bool testPassed = false;
    cout << "[EC: Edge Case Tests]" << endl;

    testPassed = e1(swa);
    if ( passed  && !testPassed) passed = false;
    

    testPassed = e2_1(swa);
    if ( passed  && !testPassed) passed = false;
    testPassed = e2_2(swa);
    if ( passed  && !testPassed) passed = false;
    
    return passed ? 0 : 1;
}

bool e1(Connection* c){
    cout << "[EC1: Invalid Sent Destination] " << endl;

    string errormsg;
    bool passed;
    int myaddr = c->getAddress();
    int badaddr = myaddr+1;

    int size = 1+strlen(test_message)+4*sizeof(int);
    message_type type = UNICAST;
    int from = myaddr;
    int to   = badaddr;
    c->sendMessage(size, type, to, test_message);
    usleep(USLEEP_TIME);
    if ( c->getMessageCount() == 1 ){
       SBMessage* msg = c->getMessage(); 
       // Check that this is indeed the correct error message.
       bool testResults = SBTestCommon::TestMessage(msg, sizeof(int)*4, INVALID_TARGET, -1, myaddr, NULL, errormsg);
       if ( testResults ) {
           cout << "passed" << endl;
           return true;
       } else {
           cout << "failed: " << errormsg << endl;
           return false;
       }
    } else {
        cout << "failed: bad number of message, got " << c->getMessageCount() << " expected 1" << endl;
        c->clearMessageQueue();
        return false;
    }
}

bool e2_1(SwitchboxAdmin* swa){
    cout << "[EC2-1: Sending message to empty group] " << endl;
    swa->def_group(1, NULL, 0);
    bool passed;
    int myaddr = swa->getAddress();
    int badaddr = myaddr+1;
    int size = 1+strlen(test_message)+4*sizeof(int);
    message_type type = MULTICAST;
    int from = myaddr;
    int to = 1;

    swa->sendMessage(size, type, to, test_message);
    usleep(USLEEP_TIME);

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
    cout << "[EC2-2: Sending message to non-existant group]" << endl;
    bool passed;
    int myaddr = swa->getAddress();
    int badaddr = myaddr+1;
    int size = 1+strlen(test_message)+4*sizeof(int);
    message_type type = MULTICAST;
    int from = myaddr;
    int to = 2;

    swa->sendMessage(size, type, to, test_message);
    usleep(USLEEP_TIME);

    // Should get an error that the group doesn't exist
    if ( swa->getMessageCount() == 1 ){
        SBMessage* msg = swa->getMessage();
        std::string errormsg;
        bool testResults = SBTestCommon::TestMessage(msg, sizeof(int)*4, INVALID_TARGET, -1, myaddr, NULL, errormsg);
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
