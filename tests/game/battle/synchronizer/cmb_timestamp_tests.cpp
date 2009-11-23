#include "Synchronizer.hpp"
#include <string>
#include <iostream>
/**
 * @addtogroup SyncTest
 * @{
 * @addtogroup cmb_timestamp_tests
 * @{
 * @file
 * @author John R. Hoare
 *
 * Tests for the new cmb_timestamp class. 
 */

using namespace std;

/** 
 * @class cmb_timestamp_tests
 * @test
 * Verifies that the extractTimestamp function works correctly with some given input. 
 */
int main(){
    cmb_timestamp cmbt;
    cout << "================================================" << endl;
    cout << "Testing cmb_timestamp Class" << endl;
    std::string ret;

    cout << "Testing with a \"normal\" message." << endl;
    ret = cmbt.extractTimestamp("120.10 foobar foop lawl okay bye");
    assert(cmbt.time==120);
    assert(cmbt.order==10);
    assert(0 == ret.compare("foobar foop lawl okay bye"));
    cout << "Passed" << endl;


    cout << "Testing with a message with lots of white-space." << endl;
    ret = cmbt.extractTimestamp("       120.10          foobar foop lawl okay bye");
    assert(cmbt.time==120);
    assert(cmbt.order==10);
    assert(0 == ret.compare("foobar foop lawl okay bye"));
    cout << "Passed" << endl;

    
    cout << "Testing with a message with the \"old style\" time stamp. This should generate a warning." << endl;
    ret = cmbt.extractTimestamp("120 foobar foop lawl okay bye");
    assert(cmbt.time==120);
    assert(cmbt.order==0);
    assert(0 == ret.compare("foobar foop lawl okay bye"));
    cout << "Passed" << endl;

    cout << "Done Testing cmb_timestamp Class" << endl;
    cout << "================================================" << endl;
}

/**
 * @}
 * @}
 */
