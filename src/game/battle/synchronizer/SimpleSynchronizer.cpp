#include "Synchronizer.hpp"

/** 
 * @file SimpleSynchronizer.cpp
 * @author John R. Hoare
 */

SimpleSynchronizer::SimpleSynchronizer(Connection * con, Interpreter * interpreter) 
    : Synchronizer(con, interpreter)
{
}

void SimpleSynchronizer::Run(){
    while(!stopRequested)
    {
        con->blockForMessage();
        SBMessage* msg = con->getMessage();
        string message_contents = (char*)msg->data;
        free(msg);
        {
            boost::mutex::scoped_lock l(timeLock);
            cTime = atoi(message_contents.c_str());
        }
        interpreter->handleEvent(message_contents);
    }
}
