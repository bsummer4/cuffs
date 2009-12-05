/// @file
/// Unit Tests for the Event class.

#include "cmb.hpp"
#include <iostream>

using namespace std;

/// @class Event
/// @test Verifies that the events are ordered in the priority queue
/// correctly.  Correctly means that the Event with the lowest time is
/// at the "top" of the queue.
void checkOrder1() {
  string sone("1.0 string one");
  string stwo("2.0 string two");
  string sthree("3.0 string three");

  cmb::Event cmbe1(sone);
  cmb::Event cmbe2(stwo);
  cmb::Event cmbe3(sthree);
  cmb::pqueue pq;
  pq.push(cmbe2);
  pq.push(cmbe3);
  pq.push(cmbe1);
  cout << "Checking that CMBEvents are ordered correctly" << endl;
  cmb::Event foo = pq.top();
  cout << foo << endl;
  assert(foo.eventOccurs.time == 1 && foo.eventOccurs.order == 0);
  while(!pq.empty()) {
    //cout << " " << pq.top();
    pq.pop();
  }
  cout << "Passed" << endl;
}

void checkOrder2() {
  string sone("1.0 string onezero");
  string stwo("1.1 string oneone");
  string sthree("1.2 string onetwo");
  string s20("2.0 string twozero");
  string s21("2.1 string twoone");
  string s22("2.2 string twotwo");

  cmb::Event cmbe1(sone), cmbe2(stwo), cmbe3(sthree), cmbe4(s20), cmbe5(s21),
             cmbe6(s22);
  cmb::pqueue pq;
  pq.push(cmbe2);
  pq.push(cmbe5);
  pq.push(cmbe3);
  pq.push(cmbe6);
  pq.push(cmbe1);
  pq.push(cmbe4);
  cout << "Checking that CMBEvents are ordered correctly" << endl;
  cmb::Event foo = pq.top();
  assert(foo.eventOccurs.time == 1 & foo.eventOccurs.order == 0);
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

