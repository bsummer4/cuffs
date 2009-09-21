#include "SwitchboxAdmin.h"
#include <iostream>
using namespace std;

SwitchboxAdmin::SwitchboxAdmin(const char* hostname,
                               const int port) : Connection(hostname,port) {}

/** 
 * Add a list of users to a group. 
 *
 * @param group The group that you wish to add the users to.
 * @param address A pointer to an array of addresses to add
 * @param addl The length of the array that *address points to.
 * 
 * @return True if succeeds, False if there is an error.
 */
bool SwitchboxAdmin::def_group(int group, int *address, int addl) {

  // Add the clients to our known groups
  set <int> clients;
  for (int ii = 0; ii < addl; ii++) clients.insert(address[ii]);
  groups.insert(make_pair <int, set < int > > (group, clients));
                  
  // Tell the switchbox about the new group
  int thissize = sizeof(admin_task_t)+sizeof(int)+sizeof(int)*addl;
  admin_message* m = (admin_message*)malloc(thissize);
  m->task = DEFINE_GROUP;
  m->group_number = group;
  memcpy(m->clients, address, addl*sizeof(int));
  sendMessage(sizeof(int)*4+thissize, ADMIN, 0, (char*)m);
  blockForMessage();
  SBMessage* msg = getMessage();
  if (debug) {
    cout << "def_group() " << endl;
    for (int ii = 0; ii < addl; ii++)
      cout << "ii = " << ii << " addr = " << address[ii] << endl;
    cout << " ROUTING TYPE = " << msg->routing_type << endl;
  }

  /// TODO: Make it go through the message queue.
  int routing_type = msg->routing_type;
  free(msg);
  return (routing_type == ADMIN_SUCCESS);
}


/**
 * Delete the group, or undefine it.
 *
 * @param group The group that you wish to remove the users from.
 * @return True if succeeds, False if there is an error.
 */
bool SwitchboxAdmin::undef_group(int group){
    int thissize = sizeof(admin_task_t)+sizeof(int);
    admin_message* m = (admin_message*)malloc(thissize);
    m->task = DELETE_GROUP;
    m->group_number = group;
    sendMessage(sizeof(int)*4+thissize, ADMIN, 0, (char*)m);

    blockForMessage();
    SBMessage* msg = getMessage();

    int routing_type = msg->routing_type;
    free(msg);
    return (routing_type == ADMIN_SUCCESS);
}

/** 
 * Create a new group
 * 
 * @param group The number to assign this group
 * @return True if group was added sucessfully, False otherwise
 */
/*
bool SwitchboxAdmin::createGroup(int group){
  int thissize = sizeof(admin_task_t)+sizeof(int)+sizeof(int**);
  admin_message* m = (admin_message*)malloc(thissize);
  m->task = RM_FROM_GROUP;
  sendMessage(sizeof(int)*4+thissize, ADMIN, 0, (char*)m);

  blockForMessage();
  SBMessage* msg = getMessage();
  /// TODO: Make it go through the message queue.
  if (msg->routing_type == ADMIN_SUCCESS){
    free(msg);
    return true;
  }
  else{
    free(msg);
    return false;
  }
}
*/
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
  /// TODO: Do this
}

/**
 * Get a list of the groups
 * 
 * @param size A pointer to an int that is set to the size of the returned array.
 * @returns A pointer to an array of ints, that are group numbers.
 */
int* SwitchboxAdmin::getGroups(int* size){
    /// TODO: Do this
}
