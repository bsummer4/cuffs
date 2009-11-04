#include "Synchronizer.hpp"
#include <iostream>
/**
 * @addtogroup SyncTest
 * @{
 * @addtogroup CMBEvent_tests
 * @{
 * @file
 * @author John R. Hoare
 *
 * Unit Tests for the CMBEvent class.
 */

using namespace std;

/**
 * @class CMBEvent
 * @test
 * Verifies that the events are ordered in the priority queue correctly.
 * Correctly means that the Event with the lowest time is at the "top" of
 * the queue.
 */
void checkOrder() {
  cmb_timestamp one = 1;
  cmb_timestamp two = 2;
  cmb_timestamp three = 3;
  string sone("string one");
  string stwo("string two");
  string sthree("string three");
  assert(one < two);

  CMBEvent cmbe1(one, sone);
  CMBEvent cmbe2(two, stwo);
  CMBEvent cmbe3(three, sthree);
  cmb_pqueue pq;
  pq.push(cmbe2);
  pq.push(cmbe3);
  pq.push(cmbe1);
  cout << "Checking that CMBEvents are ordered correctly" << endl;
  CMBEvent foo = pq.top();
  assert(foo.eventOccurs == one);
  while(!pq.empty()) {
    //cout << " " << pq.top();
    pq.pop();
  }
  //cout << endl;
}

int main() {
  cout << "================================================" << endl;
  cout << "Testing CMBEvent Class" << endl;
  //checkOrder();
  cout << "Done Testing CMBEvent Class" << endl;
  cout << "================================================" << endl;
}

/**
 * @}
 * @}
 */
