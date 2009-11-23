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
void checkOrder1() {
  cmb_timestamp one(1);
  cmb_timestamp two(2);
  cmb_timestamp three(3);
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
  cout << foo << endl;
  assert(foo.eventOccurs == one);
  while(!pq.empty()) {
    //cout << " " << pq.top();
    pq.pop();
  }
  cout << "Passed" << endl;
}

void checkOrder2() {
  cmb_timestamp onezero(1,0);
  cmb_timestamp oneone(1,1);
  cmb_timestamp onetwo(1,2);
  cmb_timestamp twozero(2,0);
  cmb_timestamp twoone(2,1);
  cmb_timestamp twotwo(2,2);
  string sone("string onezero");
  string stwo("string oneone");
  string sthree("string onetwo");
  string s20("string twozero");
  string s21("string twoone");
  string s22("string twotwo");
  assert(oneone > onezero);
  assert(onetwo > oneone);
  assert(onetwo > onezero);
  assert(onezero < oneone);
  assert(oneone < onetwo);
  assert(onezero < onetwo);

  CMBEvent cmbe1(onezero, sone);
  CMBEvent cmbe2(oneone, stwo);
  CMBEvent cmbe3(onetwo, sthree);
  CMBEvent cmbe4(twozero, s20);
  CMBEvent cmbe5(twoone, s21);
  CMBEvent cmbe6(twotwo, s22);
  cmb_pqueue pq;
  pq.push(cmbe2);
  pq.push(cmbe5);
  pq.push(cmbe3);
  pq.push(cmbe6);
  pq.push(cmbe1);
  pq.push(cmbe4);
  cout << "Checking that CMBEvents are ordered correctly" << endl;
  CMBEvent foo = pq.top();
  assert(foo.eventOccurs == onezero);
  while(!pq.empty()) {
    cout << " " << pq.top() << endl;
    pq.pop();
  }
  cout << "Passed" << endl;
}

int main() {
  cout << "================================================" << endl;
  cout << "Testing CMBEvent Class" << endl;
  checkOrder1();
  checkOrder2();
  cout << "Done Testing CMBEvent Class" << endl;
  cout << "================================================" << endl;
}

/**
 * @}
 * @}
 */
