#pragma once 

#include "Connection.hpp"
#include "Listener.hpp"

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
class Synchronizer {
    public: 
        Synchronizer(Connection &con, Listener &listener);
        virtual void Start();
        virtual unsigned int currentTime();
    private: 
        Connection &con; 
        listener &listener;
};


/** 
 * Simple synchronizer just passes any message it gets to the listener.
 */
class SimpleSynchronizer : public Synchronizer {
    public: 
        SimpleSynchronizer(Connection &con, Listener &listener);
        virtual void Start();
        virtual unsigned int currentTime();
};


/**
 * CMBSynchronizer that implements the CMB algorithm.
 */
class CMBSynchronizer : public Synchronizer {
    public:
        CMBSynchronizer(Connection &con, Listener &listener);
        virtual void Start();
        virtual unsigned int currentTime();
};

/**
 * @}
 */
