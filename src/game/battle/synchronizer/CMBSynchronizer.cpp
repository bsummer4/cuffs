#include "Synchronizer.hpp"

using namespace std;

CMBSynchronizer::CMBSynchronizer(Connection * con, Interpreter * interpreter) 
    : Synchronizer(con, interpreter)
{
}

void CMBSynchronizer::Run(){
    while(!stopRequested){
        // Get a message
        con->blockForMessage();
        SBMessage* msg = con->getMessage();
        string message_contents = (char*)msg->data;
        int process = msg->from;
        free(msg);

        // Place it into the data structure
        cmb.queueMessage(process, CMBEvent(message_contents));


        // Check to see if we can run any events
        cmb_timestamp time = cmb.getLowestTime();

        // If time is not -1 then we can advance time along.
        if ( time != -1 ){
            cmb_pqueue pq = cmb.getEvents(time);
            while ( !pq.empty() ){
                CMBEvent e = pq.top();
                pq.pop();
                interpreter->handleEvent(e.eventString);
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// CMBQueue methods below
///////////////////////////////////////////////////////////////////////////////

/** 
 * Place the given CMBEvent message onto the queue associated with the given ProcessId. 
 */
void CMBQueue::queueMessage(int processId, CMBEvent event){
    // Try to see if this processId already has a queue created
    cmb_processes::iterator it;
    it = cmbQueue.find(processId);

    if ( it == cmbQueue.end() ){
        // We do not have one already so create it
        cmb_pqueue * cmbq = new cmb_pqueue; 
        cmbQueue.insert(pair< int, cmb_pqueue* >(processId, cmbq ));

        it = cmbQueue.find(processId);
        assert(it != cmbQueue.end());
    }

    // Okay now "it" points to the queue for this process
    it->second->push(event);
}


/** 
 * getLowestTime() returns the lowest time that the GVT can be advanced to. 
 * If there is none that it can be advanced to, it will return a -1. 
 *
 * @return -1 When the GVT can not be advanced, otherwise the function
 * will return the farthest the GVT can be advanced. 
 */
cmb_timestamp CMBQueue::getLowestTime(){
    cmb_processes::iterator it;
    cmb_timestamp lowestTime = -1;
    for (it = cmbQueue.begin(); it != cmbQueue.end(); it++){
        if ( it->second->empty() ){
            return -1;  
        }
        else {
            cmb_timestamp ittime = it->second->top().eventOccurs; 
            if ( ittime < lowestTime || lowestTime == -1 ){
                lowestTime = ittime;
            }
        }
    }
    return lowestTime;
}

/**
 * Get all the events in a vector up to the time given as an argument. 
 *
 * @param tillTime The time, likely taken from getLowestTime() function. 
 * @returns A vector of all the runnable CMBEvent's 
 */
cmb_pqueue CMBQueue::getEvents(cmb_timestamp tillTime){
    cmb_pqueue retval;
    cmb_processes::iterator it;
    if ( getLowestTime() < tillTime )
        return retval;
    for (it = cmbQueue.begin(); it != cmbQueue.end(); it++){
        while( !it->second->empty() ){
            CMBEvent e = it->second->top();
            if ( tillTime >= e.eventOccurs ){
                it->second->pop();
                retval.push(e);
            }
            else {
                break;
            }
        }
    }
    return retval;
}

///////////////////////////////////////////////////////////////////////////////
// CMBEvent methods below
///////////////////////////////////////////////////////////////////////////////

/** 
 * Constructor that has the time and event already parsed out
 */
CMBEvent::CMBEvent(cmb_timestamp _time, std::string _event){
    this->eventOccurs = _time;
    this->eventString = _event;
}

/**
 * Constructor that parses the time out of the message. 
 */
CMBEvent::CMBEvent(string message){
    this->eventOccurs = atoi(message.c_str());
    this->eventString = message;
}

/**
 * Default constructor.
 */
CMBEvent::CMBEvent(){
}
