#include <string>
#include <queue>

extern "C" {
#include "switchbox_client.h"
#include <poll.h>
#include <pthread.h>
}

class Connection{
    public:
        Connection(const char* hostname, const int port);
        bool  sendMessage(int size, int type, int to, char *string);
        //char* receive_string(int* size, int* type, int* from, int* to);
        SBMessage * getMessage();
        void start();
        void stop();
        void messageUpdate();
        int getMessageCount();
        int getAddress();
        bool isRunning();
        void blockForMessage();
    private:
        void handleAnnounceMessage(SBMessage * msg);
    private:
        Socket s;
        pthread_mutex_t rlock;
        std::queue<SBMessage*> receive_queue;
        pthread_mutex_t slock;
        std::queue<SBMessage*> send_queue;
        pthread_cond_t blocker;
        int from;
        int port;
        bool running;
        std::string hostname;
        pthread_t tid;
        struct pollfd fds_[1];
};
