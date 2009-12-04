/// @file
/// Unit Tests for cmb::Queue

#include "cmb.hpp"
#include <iostream>
#include <vector>

using namespace std;

vector<int> twoClients;
vector<int> oneClient;

/// @class cmb::Queue
/// @test Tests with one process that the queue orders the messages correctly.
void testOne() {
  cout << "Testing with one process" << endl;
  cmb::Event cmbe1("1.0 foo");
  cmb::Event cmbe2("2.0 foo");
  cmb::Queue q(oneClient);
  assert(q.queueMessage(1, cmbe1));
  assert(q.queueMessage(1, cmbe2));
  cmb::Timestamp time = q.getLowestTime();
  assert(time==1);
}

/// @class cmb::Queue
/// @test Tests with two processes that the queue orders the messages
/// correctly.
void testTwo() {
  cout << "Testing with two processes" << endl;
  cmb::Event cmbe1("1.0 foo");
  cmb::Event cmbe2("2.0 foo");
  cmb::Queue q(twoClients);
  assert(q.queueMessage(1, cmbe1));
  assert(q.queueMessage(2, cmbe2));
  cmb::Timestamp time = q.getLowestTime();
  //cout << "time (should be 1): " << time;
  assert(time==1);
}

/// @class cmb::Queue
/// @test Tests with two processes that the the lowest Time is
/// correct, and the correct runnable events are returned.
void testThree() {
  cout << "Testing getEvents()" << endl;
  cmb::Event cmbe1(1,"foo");
  cmb::Event cmbe2(2,"foo");
  cmb::Queue q(twoClients);
  assert(q.queueMessage(1, cmbe1));
  assert(q.queueMessage(2, cmbe2));
  cmb::Timestamp time = q.getLowestTime();
  //cout << "time == " << time << endl;
  cmb::pqueue pq = q.getEvents(time);
  //cout << "pq.size() == " << pq.size() << endl;
  assert(pq.size()==1);
  assert(pq.top().eventOccurs==1);
}

int main() {
  twoClients.push_back(1);
  twoClients.push_back(2);
  oneClient.push_back(1);
  cout << "================================================" << endl;
  cout << "Testing CMBQueue Class" << endl;
  testOne();
  testTwo();
  testThree();
  cout << "Done Testing CMBQueue Class" << endl;
  cout << "================================================" << endl;
}

/// @}
/// @}
