#include "SwitchboxAdmin.h"


SwitchboxAdmin::SwitchboxAdmin(const char* hostname, const int port) : Connection(hostname,port){

}


/** 
 * Add a list of users to a group. 
 *
 * @param group The group that you wish to add the users to.
 * @param address A pointer to an array of addresses to add
 * @param addl The length of the array that *address points to.
 * 
 * @return True if succeeds, False if there is an error.
 */
bool SwitchboxAdmin::addToGroup(int group, int *address, int addl){


}


/**
 * Remove a list of users from a group.
 *
 * @param group The group that you wish to remove the users from.
 * @param address A pointer to an array of addresses to remove 
 * @param addl  The length of the array that *address points to.
 * @return True if succeeds, False if there is an error.
 */
bool SwitchboxAdmin::removeFromGroup(int group, int *address, int addl){

}

/** 
 * Create a new group
 * 
 * @param group The number to assign this group
 * @return True if group was added sucessfully, False otherwise
 */
bool SwitchboxAdmin::createGroup(int group){

}

/**
 * Get a list of all the group members in a requested group.
 * 
 * @param group The group number to query. 
 * @param size A pointer to an int that returns the size returned. 
 * Set to -1 if there is an error.
 *
 * @return A pointer to an array of ints, that are the addresses. NULL if 
 * an error occurs.
 */
int* SwitchboxAdmin::getGroupMembers(int group, int *size){

}

/**
 * Get a list of the groups
 * 
 * @param size A pointer to an int that is set to the size of the returned array.
 * @returns A pointer to an array of ints, that are group numbers.
 */
int* SwitchboxAdmin::getGroups(int* size){

}
