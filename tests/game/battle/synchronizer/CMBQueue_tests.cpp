#include "Synchronizer.hpp"
#include <iostream>

using namespace std;

void testOne(){
    cout << "Testing with one process" << endl;
    CMBEvent cmbe1(1,"foo");
    CMBEvent cmbe2(2,"foo");
    CMBQueue q; 
    q.queueMessage(1, cmbe1);
    q.queueMessage(1, cmbe2);
    cmb_timestamp time = q.getLowestTime();
    //cout << "time: " << time;
    assert(time==1);
}

void testTwo(){
    cout << "Testing with two processes" << endl;
    CMBEvent cmbe1(1,"foo");
    CMBEvent cmbe2(2,"foo");
    CMBQueue q; 
    q.queueMessage(1, cmbe1);
    q.queueMessage(2, cmbe2);
    cmb_timestamp time = q.getLowestTime();
    //cout << "time: " << time;
    assert(time==1);
}

void testThree(){
    cout << "Testing getEvents()" << endl;
    CMBEvent cmbe1(1,"foo");
    CMBEvent cmbe2(2,"foo");
    CMBQueue q; 
    q.queueMessage(1, cmbe1);
    q.queueMessage(2, cmbe2);
    cmb_timestamp time = q.getLowestTime();
    //cout << "time == " << time << endl;
    cmb_pqueue pq = q.getEvents(time);
    //cout << "pq.size() == " << pq.size() << endl;
    assert(pq.size()==1);
    assert(pq.top().eventOccurs==1);
}

int main(){
    cout << "================================================" << endl;
    cout << "Testing CMBQueue Class" << endl;
    testOne();
    testTwo();
    testThree();
    cout << "Done Testing CMBQueue Class" << endl;
    cout << "================================================" << endl;
}
