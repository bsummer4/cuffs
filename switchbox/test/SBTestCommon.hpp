#pragma once

static const int SWITCHBOX_PORT = 5151;
static const int USLEEP_TIME = 100000;

char* HOST= (char*) "localhost";
char* test_message = (char*) "hello";

namespace SBTestCommon {
// Return True if correct, false if failed
bool TestMessage(SBMessage * msg, int size, message_type_t type, int from,
                 int to, char* message, std::string& errormsg);
};
