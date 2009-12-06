#include "SwitchboxAdmin.hpp"
extern "C" {
#include <sys/time.h>
#include <errno.h>
}

SwitchboxAdmin::SwitchboxAdmin(const char* hostname,
                               const int port) : Connection(hostname,port) {
  pthread_mutex_init(&groups_lock, NULL);
  pthread_cond_init(&groups_cond, NULL);
}


/**
 * Handle group announcement messages and whatever the parent class
 * handled.
 */
void SwitchboxAdmin::handleAnnounceMessage(SBMessage * msg) {
  Connection::handleAnnounceMessage(msg);

  admin_message *admin = (admin_message*) msg->data;

  if(debug)
    printf("%p, %d, %d, %d\n",
           msg, admin->task, admin->group_number, msg->size);

  if(admin->task == DEFINE_GROUP) {
    pthread_mutex_lock(&groups_lock);
    groups.insert(admin->group_number);
    pthread_mutex_unlock(&groups_lock);
    pthread_cond_signal(&groups_cond);
  }

  if(admin->task == DELETE_GROUP) {
    pthread_mutex_lock(&groups_lock);
    groups.erase(admin->group_number);
    pthread_mutex_unlock(&groups_lock);
    pthread_cond_signal(&groups_cond);
  }
}


/**
 * Create a group group_id with num_clients addresses taken from
 * addresses.  Return true if said group now exists in the switchbox.
 */
bool SwitchboxAdmin::makeGroup(int group_id, int *addresses,
                               int num_clients) {
  SBMessage *message = make_group(getAddress(), group_id, num_clients,
                                  addresses);
  bool success;
  sendMessage(message);

  struct timeval tv;
  struct timespec ts;
  gettimeofday(&tv, NULL);
  ts.tv_sec = tv.tv_sec + GROUP_REQUEST_TIMEOUT/1000;
  ts.tv_nsec = tv.tv_usec * 1000 + (GROUP_REQUEST_TIMEOUT)%1000 * 1000000;

  pthread_mutex_lock(&groups_lock);
  groups.erase(group_id);

  /// @TODO If there are two entities creating groups (say A and B), and
  ///      A creates a group and then B creates another group
  ///      immediately afterwords. B will get A's announcement message
  ///      first; It will signal the condition variable. since B's
  ///      group wont be setup yet (in 'groups'). B will see this as a
  ///      failure and return false.
  pthread_cond_timedwait(&groups_cond, &groups_lock, &ts);
  success = (groups.count(group_id) != 0);
  pthread_mutex_unlock(&groups_lock);

  return success;
}


/**
 * Remove group_id from switchbox.  @return true when it's been
 * removed.
 */
bool SwitchboxAdmin::removeGroup(int group_id) {
  if(!groups.count(group_id)) return true;
  SBMessage *m = remove_group(getAddress(), group_id);
  bool success;
  sendMessage(m);

  struct timeval tv;
  struct timespec ts;
  gettimeofday(&tv, NULL);
  ts.tv_sec = tv.tv_sec + GROUP_REQUEST_TIMEOUT/1000;
  ts.tv_nsec = tv.tv_usec * 1000 + (GROUP_REQUEST_TIMEOUT)%1000 * 1000000;

  /// @TODO If there are two entities creating groups (say A and B), and
  ///      A creates a group and then B creates another group
  ///      immediately afterwords. B will get A's announcement message
  ///      first; It will signal the condition variable. since B's
  ///      group wont be setup yet (in 'groups'). B will see this as a
  ///      failure and return false.
  pthread_mutex_lock(&groups_lock);
  pthread_cond_timedwait(&groups_cond, &groups_lock, &ts);
  success = (groups.count(group_id) == 0);
  pthread_mutex_unlock(&groups_lock);

  return success;
}
