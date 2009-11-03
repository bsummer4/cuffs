#include <string>
#include "switchbox_client.h"


using namespace std;

namespace SBTestCommon {

bool TestMessage(SBMessage * msg, int size, message_type_t type, int from, int to, char* message, std::string& errormsg) {
    bool passed = true;
    if (msg->size != size) {
        passed = false;
        errormsg += "Size wrong\n";
        //errormsg += string("Size is wrong. Expected ") + size + string(" got ") + msg->size + string("\n");
    }
    if (msg->routing_type != type) {
        passed = false;
        errormsg += "routing_type wrong\n";
        //errormsg += string("Type is wrong. Expected ") + type + string(" got ") + msg->type + string("\n");
    }
    if (msg->from != from) {
        passed = false;
        errormsg += "from wrong\n";
        //errormsg += string("From is wrong. Expected ") + from + string(" got ") + msg->from + string("\n");
    }
    if (msg->to != to) {
        passed = false;
        errormsg += "to wrong\n";
        //errormsg += string("To is wrong. Expected ") + to + string(" got ") + msg->to + string("\n");
    }
    if (passed && message) {
        int dataLength = size - sizeof(int) *4;
        char * p;
        char* q;
        p = msg->data;
        q = message;
        for (int i = 0; i < dataLength; i++) {
            if (* (p++) != * (q++)) {
                passed = false;
                errormsg += "Message does not match\n";
                break;
            }
        }
    }
    return passed;
}
}
