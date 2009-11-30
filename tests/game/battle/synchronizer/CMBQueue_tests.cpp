#include "Synchronizer.hpp"
#include <iostream>
#include <vector>
/**
 * @addtogroup SyncTest
 * @{
 * @addtogroup CMBQueue_tests
 * @{
 * @file
 * @author John R. Hoare
 *
 * Unit Tests for the CMBQueue class.
 */

using namespace std;

vector<int> twoClients;
vector<int> oneClient;


/**
 * @class CMBQueue
 *
 * @test Tests with one process that the queue orders the messages correctly.
 */
void testOne() {
  cout << "Testing with one process" << endl;
  CMBEvent cmbe1(1,"foo");
  CMBEvent cmbe2(2,"foo");
  CMBQueue q(oneClient);
  assert(q.queueMessage(1, cmbe1));
  assert(q.queueMessage(1, cmbe2));
  cmb_timestamp time = q.getLowestTime();
  //cout << "time (should be 1): " << time;
  assert(time==1);
}

/**
 * @class CMBQueue
 *
 * @test Tests with two processes that the queue orders the messages correctly.
 */
void testTwo() {
  cout << "Testing with two processes" << endl;
  CMBEvent cmbe1(1,"foo");
  CMBEvent cmbe2(2,"foo");
  CMBQueue q(twoClients);
  q.queueMessage(1, cmbe1);
  q.queueMessage(2, cmbe2);
  cmb_timestamp time = q.getLowestTime();
  //cout << "time (should be 1): " << time;
  assert(time==1);
}

/**
 * @class CMBQueue
 *
 * @test Tests with two processes that the the lowest Time is correct, and the
 * correct runnable events are returned.
 */
void testThree() {
  cout << "Testing getEvents()" << endl;
  CMBEvent cmbe1(1,"foo");
  CMBEvent cmbe2(2,"foo");
  CMBQueue q(twoClients);
  q.queueMessage(1, cmbe1);
  q.queueMessage(2, cmbe2);
  cmb_timestamp time = q.getLowestTime();
  //cout << "time == " << time << endl;
  cmb_pqueue pq = q.getEvents(time);
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


/**
 * @}
 * @}
 */
