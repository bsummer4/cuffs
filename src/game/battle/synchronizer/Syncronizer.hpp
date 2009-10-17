#pragma once 

#include "Connection.hpp"
#include "Interpreter.hpp"
#include "Threaded.hpp"

/** 
 * @defgroup Synchronizer Synchronizer
 * @ingroup Battle
 * The Synchronizer portion of the CMB triad. 
 */

/** 
 * @addtogroup Synchronizer
 * @{
 */

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
 * CMBSynchronizer that implements the CMB algorithm.
 */
class CMBSynchronizer : public Synchronizer {
    public:
        CMBSynchronizer(Connection * con, Interpreter * interpreter);
        virtual void Run();
};

/**
 * @}
 */
