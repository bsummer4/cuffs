#pragma once

#include "Connection.hpp"
#include <set>
#include <pthread.h>

static const int GROUP_REQUEST_TIMEOUT = 100; // 100 Ms timeout

/**
 * A class to handle administrative things for the switchbox, ie
 * adding/removing groups etc.
 */
class SwitchboxAdmin : public Connection {
public:
    SwitchboxAdmin(const char* hostname, const int port);
    bool makeGroup(int group, int *address, int addl);
    bool removeGroup(int group);
    virtual void handleAnnounceMessage(SBMessage * msg);

private:
    std::set <int> groups;
    pthread_mutex_t groups_lock;
    pthread_cond_t  groups_cond;
};
