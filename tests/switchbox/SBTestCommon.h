#pragma once

namespace SBTestCommon{
    bool TestMessage(SBMessage * msg, int size, message_type type, int from, int to, char* message, std::string& errormsg);
};
