#pragma once

#include "Connection.h"
#include <map>
#include <set>

/** @addtogroup ClientLib
 * @{
 */


/**
 * A class to handle administrative things for the switchbox, ie adding/removing groups etc.
 */
class SwitchboxAdmin : public Connection{
    public:
        SwitchboxAdmin(const char* hostname, const int port);
        /// Create a new group
        bool createGroup(int group);
        /// Remove a group
        bool removeGroup(int group);
        /// Get a list of the members of a given group
        int* getGroupMembers(int group, int *size);
        /// Get a list of the groups
        int* getGroups(int* size);
        /// Add the addresses to a group
        bool def_group(int group, int *address, int addl);
        /// Undefine group
        bool undef_group(int group);
   private:
        std::map <int, std::set <int> > groups;
};


/**
 * @}
 */
