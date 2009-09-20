#include "Connection.h"
#include "SBTestCommon.h"
#include <vector>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <iostream>

using namespace std;

void * update_thread(void* arg);
void spawn_pair();

// From http://www.cs.utah.edu/dept/old/texinfo/glibc-manual-0.02/library_19.html
int
timeval_subtract (struct timeval *result, struct timeval *x, struct timeval *y)
{
    /* Perform the carry for the later subtraction by updating y. */
    if (x->tv_usec < y->tv_usec) {
        int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
        y->tv_usec -= 1000000 * nsec;
        y->tv_sec += nsec;
    }
    if (x->tv_usec - y->tv_usec > 1000000) {
        int nsec = (y->tv_usec - x->tv_usec) / 1000000;
        y->tv_usec += 1000000 * nsec;
        y->tv_sec -= nsec;
    }

    /* Compute the time remaining to wait.
       tv_usec is certainly positive. */
    result->tv_sec = x->tv_sec - y->tv_sec;
    result->tv_usec = x->tv_usec - y->tv_usec;

    /* Return 1 if result is negative. */
    return x->tv_sec < y->tv_sec;
}

class ConPair{
    public: 
        ConPair(Connection* sender, Connection* receiver){
            s = sender;
            r = receiver;
            pthread_mutex_init(&lock, NULL);
        }
        ConPair(){
            s = new Connection(HOST, SWITCHBOX_PORT);
            r = new Connection(HOST, SWITCHBOX_PORT);
            s->start();
            r->start();
            pthread_mutex_init(&lock, NULL);
        }
        void start(){
            running = true;
            pthread_create(&tid, NULL, update_thread, (void*)this);
        }
        void stop(){
            running = false;
            pthread_join(tid, NULL);
        }
        void update(){
            struct timeval sent;
            struct timeval rec;
            gettimeofday(&sent, NULL);
            int size = 4*sizeof(int) + sizeof(struct timeval);
            message_type type = UNICAST;
            int to = r->getAddress();
            s->sendMessage(size, type, to, (char*)&sent);
            r->blockForMessage();
            SBMessage * msg = (SBMessage*)malloc(size);
            msg = r->getMessage();
            gettimeofday(&rec, NULL);
            struct timeval diff_val;
            timeval_subtract(&diff_val, &sent, &rec);
            pthread_mutex_lock(&lock);
            latencies.push_back(diff_val);
            pthread_mutex_unlock(&lock);

        }
        double getAverageLatency(){
            struct timeval sum = {0,0};
            pthread_mutex_lock(&lock);
            for (int i = 0; i < latencies.size(); i++){
                sum.tv_sec += latencies[i].tv_sec;
                sum.tv_usec += latencies[i].tv_usec;
            }
            double msec = sum.tv_sec * 1000 + sum.tv_usec * 0.001;
            if ( latencies.size() > 0 ){
                msec /= latencies.size();
            }
            else { 
                msec = 1000000;
            }
            latencies.clear();
            pthread_mutex_unlock(&lock);
            return msec;
        }
        bool isRunning() { return running; }
    private:
        Connection * s;
        Connection * r;
        vector<struct timeval> latencies;
        pthread_mutex_t lock;
        pthread_t tid;
        bool running;
};

vector<ConPair*> clients;

void * update_thread(void* arg){
    ConPair * cp = (ConPair*)arg;
    while(cp->isRunning()){
        cp->update();
    }
    pthread_exit(NULL);
}

int main(){
    int MAXPAIRS = 100;

    double sum;
    double avgLatency;

    while ( clients.size() < MAXPAIRS ){
        spawn_pair();
        // Let the clients run
        sleep(5);
        sum = 0;
        for (int i = 0; i < clients.size(); i++){
            sum = clients[i]->getAverageLatency();
        }
        avgLatency = sum/clients.size();
        cout << "[Pairs: " << clients.size() << "] Latency: " << avgLatency << endl;
    }
}

void spawn_pair(){
    ConPair* c = new ConPair();
    clients.push_back(c);
}


