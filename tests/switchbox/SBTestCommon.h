#pragma once

static const int USLEEP_TIME = 20000;

const char* HOST="localhost";
char* test_message = "hello";

namespace SBTestCommon{
    // Return True if correct, false if failed
    bool TestMessage(SBMessage * msg, int size, message_type type, int from, int to, char* message, std::string& errormsg);
};
