#include "Threaded.hpp"

Threaded::Threaded()
    : stopRequested(false)
{
}

/**
 * Starts a thread calling the Run() function. 
 */
void Threaded::Start(){
    assert(!runThread);
    runThread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&Threaded::Run, this)));
}

/**
 * Sets the stopRequested bool to true, and will join the thread. 
 * This expects the running thread to respect the stopRequested bool. 
 */
void Threaded::Stop(){
    assert(runThread);
    stopRequested = true;
    runThread->join();
}
