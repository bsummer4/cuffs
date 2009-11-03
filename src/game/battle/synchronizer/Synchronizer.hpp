#pragma once
/**
 * @file Synchronizer.hpp
 * @author John R. Hoare
 */

#include "Connection.hpp"
#include "Interpreter.hpp"
#include "Threaded.hpp"

#include <map>
#include <string>
#include <queue>

/**
 * @defgroup Synchronizer Synchronizer
 * @ingroup Battle
 * The Synchronizer portion of the CMB triad.
 */

/**
 * @addtogroup Synchronizer
 * @author John R. Hoare
 * @{
 */

typedef int cmb_timestamp;

/**
 * The base class for the synchronizer that defines the interface.
 */
class Synchronizer : public Threaded {
public:
    Synchronizer(Connection * con, Interpreter * interpreter);
    virtual unsigned int currentTime();
protected:
    Connection * con;
    Interpreter * interpreter;
    boost::mutex timeLock;
    unsigned int cTime;
};


/**
 * Simple synchronizer just passes any message it gets to the listener.
 */
class SimpleSynchronizer : public Synchronizer {
public:
    SimpleSynchronizer(Connection * con, Interpreter * interpreter);
    virtual void Run();
};

/**
 * @addtogroup CMBSynchronizer
 * @author John R. Hoare
 *
 * All the classes associated with the CMB objects for the CMBSynchronizer
 * @{
 */

/**
 * A class that contains the timestamp as well as the string representing
 * an event.
 *
 * This function holds a "CMB" event, and can be compared with other CMBEvent
 * objects in order to function correctly in the priority queue.
 */
class CMBEvent {
public:
    CMBEvent();
    CMBEvent(std::string message);
    CMBEvent(cmb_timestamp time, std::string event);
    // Need to do this to use in priority queue.
    // http://www.codeguru.com/cpp/tic/tic0229.shtml
    /// This is actually backwards so that it gets ordered in the way
    /// we like for the priority queue.
    friend bool operator<( const CMBEvent& x, const CMBEvent& y) {
        return ( x.eventOccurs > y.eventOccurs );
    }

    /// So we can prettyprint this object
    friend std::ostream& operator<<(std::ostream& output, const CMBEvent& c) {
        output << "Time: " << c.eventOccurs << " String: " << c.eventString;
        return output;
    }

    /// Timestamp of when the event occured
    cmb_timestamp eventOccurs;
    /// The actual string (including the timestamp) of the event.
    std::string eventString;
};

/// A priority queue for CMBEvent objects
typedef std::priority_queue< CMBEvent > cmb_pqueue;
/// A map of cmb_pqueue's that are to be indexed on the process's name.
typedef std::map< int, cmb_pqueue* > cmb_processes;


/**
 * A priority queue class that manages placing events into appropriate
 * queues as well as determining if the global time can be advanced.
 *
 */
class CMBQueue {
public:
    CMBQueue() {};
    void queueMessage(int processId, CMBEvent cmbe);
    void removeProcess(int processId);
    cmb_timestamp getLowestTime();
    cmb_pqueue getEvents(cmb_timestamp tillTime);
private:
    cmb_processes cmbQueue;
};

/**
 * CMBSynchronizer that implements the CMB algorithm.
 */
class CMBSynchronizer : public Synchronizer {
public:
    CMBSynchronizer(Connection * con, Interpreter * interpreter);
    virtual void Run();
    void startSendToInt();
private:
    CMBQueue cmb;
protected:
    bool send_to_int;
};

/**
 * @}
 * @}
 */
