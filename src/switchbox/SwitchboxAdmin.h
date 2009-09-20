#pragma once

#include "Connection.h"

/** @addtogroup ClientLib
 * @{
 */


/**
 * A class to handle administrative things for the switchbox, ie adding/removing groups etc.
 */
class SwitchboxAdmin : public Connection{
    public:
        SwitchboxAdmin(const char* hostname, const int port);
        /// Add the addresses to a group
        bool addToGroup(int group, int *address, int addl);
        /// Remove the addresses to a group
        bool removeFromGroup(int group, int *address, int addl);
        /// Create a new group
        bool createGroup(int group);
        /// Remove a group
        bool removeGroup(int group);
        /// Get a list of the members of a given group
        int* getGroupMembers(int group, int *size);
        /// Get a list of the groups
        int* getGroups(int* size);
};


/**
 * @}
 */
