#include "Synchronizer.hpp"
#include <boost/regex.hpp>
#include <iostream>

using namespace std;
/**
 * @file CMBSynchronizer.cpp
 * @author John R. Hoare
 */


// CMB TIMESTAMP STUFF
cmb_timestamp::cmb_timestamp(int time, int order){
    this->time = time;
    this->order = order;
};

bool operator<(const cmb_timestamp& x, const cmb_timestamp& y){
    if ( x.time == y.time )
        return x.order < y.order;
    else 
        return x.time < y.time;
}
bool operator==(const cmb_timestamp& x, const cmb_timestamp& y){
    return (x.time == y.time && x.order == y.order );
}
bool operator>(const cmb_timestamp& x, const cmb_timestamp& y){
    if ( x.time == y.time )
        return x.order > y.order;
    else 
        return x.time > y.time;
}
bool operator>=(const cmb_timestamp& x, const cmb_timestamp& y){
    return (x > y) || (x == y);
}
bool operator<=(const cmb_timestamp& x, const cmb_timestamp& y){
    return (x < y) || (x == y);
}
std::ostream& operator<<(std::ostream& output, const cmb_timestamp& c){
    output << c.time << "." << c.order;
    return output;
}


/** 
 * Extract the timestamp from the given event string. 
 *
 * @param event The event string that has a timestamp of ###.### (number.number)
 *   This string is unharmed. 
 * @returns The string with just the message, the timetamp is stripped off. Also, 
 * any whitespace between the timestamp and the event is also stripped off. 
 */
std::string cmb_timestamp::extractTimestamp(std::string event){
    // The regular expression that defines our timestamp + message
    // It will allow (and trim) whitespace before and after the timestamp.
    boost::regex e("^[[:s:]]*([0-9]+)\\.([0-9]+)[[:s:]]+(.*)$");
    boost::regex e2("^[[:s:]]*([0-9]+)[[:s:]]+(.*)$");
    boost::cmatch res;
    if (boost::regex_match(event.c_str(), res, e)){
        //res[0] contains whole string
        //res[1] contains the "time"
        //res[2] contains the "order"
        //res[3] contains the "message"
        /*
        cout << "res.size() " << res.size() << endl;
        cout << "res[0] " << res[0] << endl;
        cout << "res[1] " << atoi(res[1].first) << endl;
        cout << "res[2] " << atoi(res[2].first) << endl;
        cout << "res[3] " << res[3].first << endl;
        */
        time = atoi(res[1].first);
        order = atoi(res[2].first);
        return std::string(res[3].first);
    }
    else if (boost::regex_match(event.c_str(), res, e2)){
        cerr << "Warning: Got old format of timestamp" << endl;
        time = atoi(res[1].first);
        order = 0;
        return std::string(res[2].first);
    }
    else {
        cerr << "Got Bad Message : \"" << event << "\"" << endl;
        assert(false);
    }
}
// END CMB TIMESTAMP STUFF


/** 
 * Set num_of_clients if you know how many clients are going to join. The synchronizer will start 
 * to run as soon as it "knows" this many clients. Otherwise set it to -1 and use the sendToInt()
 * function to start the synchronizer. 
 */
CMBSynchronizer::CMBSynchronizer(Connection * con, Interpreter * interpreter, int num_of_clients)
  : Synchronizer(con, interpreter) {
  send_to_int = false;
  this->num_of_clients = num_of_clients;
}

void CMBSynchronizer::Run() {
  while(!stopRequested) {
    // Get a message
    con->blockForMessage();
    SBMessage* msg = con->getMessage();
    string message_contents = (char*)msg->data;
    int process = msg->from;
    //cout << "process = " << process << endl;
    free(msg);

    // Place it into the data structure
    cmb.queueMessage(process, CMBEvent(message_contents));

    //cerr << "Count = " << cmb.getKnownProcessCount() << endl;
    //cerr << "num_of_clients = " << num_of_clients << endl;
    if ( num_of_clients > 0 && cmb.getKnownProcessCount() == num_of_clients ){
        send_to_int = true;
    }

    if(send_to_int) {
      // Check to see if we can run any events
      cmb_timestamp t = cmb.getLowestTime();

      //cout << "Lowest time is " << t << endl;

      // If time is not -1 then we can advance time along.
      if(t.time > -1 && t.order==0) {
        cmb_pqueue pq = cmb.getEvents(t);
        while(!pq.empty()) {
          CMBEvent e = pq.top();
          pq.pop();
          interpreter->handleEvent(e.eventString);
        }
        boost::mutex::scoped_lock l(timeLock);
        cTime = t.time;
      }
    }
  }
}

/**
 * Tell the Synchronizer that it is okay to start sending messages
 * along to the interpreter to be interpreted.
 *
 * Basically, you will call @ref Start to start the thread, and it
 * will begin appending messages to the queue. However, this needs
 * to be called once everybody is connected and you can start running.
 */
void CMBSynchronizer::startSendToInt() {
  cerr << "I know of " << cmb.getKnownProcessCount() << " clients" << endl;
  send_to_int = true;
}

///////////////////////////////////////////////////////////////////////////////
// CMBQueue methods below
///////////////////////////////////////////////////////////////////////////////

/**
 * Place the given CMBEvent message onto the queue associated with the given ProcessId.
 *
 * @param processId The Id of the process that this message is associated with
 * @param event The CMBEvent object representing the event.
 */
void CMBQueue::queueMessage(int processId, CMBEvent event) {
  // Try to see if this processId already has a queue created
  cmb_processes::iterator it;
  it = cmbQueue.find(processId);

  if(it == cmbQueue.end()) {
    // We do not have one already so create it
    cmb_pqueue * cmbq = new cmb_pqueue;
    cmbQueue.insert(pair< int, cmb_pqueue* >(processId, cmbq));

    it = cmbQueue.find(processId);
    assert(it != cmbQueue.end());
  }

  // Okay now "it" points to the queue for this process
  it->second->push(event);
  //cout << "pushed message: process " << processId << endl;
}


/**
 * getLowestTime() returns the lowest time that the GVT can be advanced to.
 * If there is none that it can be advanced to, it will return a -1.
 *
 * @return -1 When the GVT can not be advanced, otherwise the function
 * will return the farthest the GVT can be advanced.
 */
cmb_timestamp CMBQueue::getLowestTime() {
  cmb_processes::iterator it;
  cmb_timestamp lowestTime;
  cmb_timestamp lowestTimeHighestOrder;
  //cerr << endl;
  for(it = cmbQueue.begin(); it != cmbQueue.end(); it++) {
    if(it->second->empty()) {
      // Everyone has to have at least SOME message
      return cmb_timestamp(-1);
      break;
    } else {
      //cerr << it->second->top() << endl;
      cmb_timestamp ittime = it->second->top().eventOccurs;
      if ( ittime.time <= lowestTime.time || lowestTime == -1 ){
        lowestTime = ittime;
        // This is so that we know that every process has its 0 for the lowest time 
        // We'll check at the end to make sure everything is fine.
        if ( ittime.order > lowestTimeHighestOrder.order ){
            lowestTimeHighestOrder = ittime;
        }
      }
    }
  }
  // Check to make sure all the clients are "ready" at this given timestep.
  if ( lowestTimeHighestOrder.order != 0 && lowestTimeHighestOrder.time == lowestTime.time )
      return cmb_timestamp(-1);
  else
      return lowestTime;
}

/**
 * Get all the events in a vector up to the time given as an argument.
 *
 * @param tillTime The time, likely taken from getLowestTime() function.
 * @returns A vector of all the runnable CMBEvent's
 */
cmb_pqueue CMBQueue::getEvents(cmb_timestamp tillTime) {
  cmb_pqueue retval;
  cmb_processes::iterator it;
  if(getLowestTime() < tillTime)
    return retval;
  for(it = cmbQueue.begin(); it != cmbQueue.end(); it++) {
    while(!it->second->empty()) {
      CMBEvent e = it->second->top();
      // Just look at the time, because the timebuckets are all or nothing.
      if(tillTime.time >= e.eventOccurs.time) {
        it->second->pop();
        retval.push(e);
      } else {
        break;
      }
    }
  }
  return retval;
}

/**
 * Get the number of processes that the Queue Knows about.
 */
int CMBQueue::getKnownProcessCount(){
    return cmbQueue.size();
}

///////////////////////////////////////////////////////////////////////////////
// CMBEvent methods below
///////////////////////////////////////////////////////////////////////////////

/**
 * Constructor that has the time and event already parsed out
 */
CMBEvent::CMBEvent(cmb_timestamp _time, std::string _event) {
  this->eventOccurs = _time;
  this->eventString = _event;
}

/**
 * Constructor that parses the time out of the message.
 */
CMBEvent::CMBEvent(std::string message) {
  //this->eventOccurs = atoi(message.c_str());
  this->eventString = eventOccurs.extractTimestamp(message);
}

/**
 * Default constructor.
 */
CMBEvent::CMBEvent() {
}
