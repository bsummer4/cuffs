#pragma once 

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

class CMBEvent{
    public:
        CMBEvent();
        CMBEvent(std::string message);
        CMBEvent(cmb_timestamp time, std::string event);
        // Need to do this to use in priority queue. 
        // http://www.codeguru.com/cpp/tic/tic0229.shtml
        friend bool operator<( const CMBEvent& x, const CMBEvent& y){
            return ( x.eventOccurs > y.eventOccurs );
        }

        friend ostream& operator<<(ostream& output, const CMBEvent& c) {
            output << "Time: " << c.eventOccurs << " String: " << c.eventString;
            return output;
        }

        cmb_timestamp eventOccurs;
        std::string eventString;
};

typedef std::priority_queue< CMBEvent > cmb_pqueue;
typedef std::map< int, cmb_pqueue* > cmb_processes; 

        
class CMBQueue{
    public: 
        CMBQueue(){};
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
    private:
        CMBQueue cmb;
};

/**
 * @}
 */
