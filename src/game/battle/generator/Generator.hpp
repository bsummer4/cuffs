#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <unistd.h>
extern "C" {
#include "message.h"
#include "switchbox_client.h"
}
#include "Connection.hpp"
using namespace std;

//Global flag debug
bool debug;

/** 
 * @defgroup Game Game
 * Everything that goes into the game backend. 
 */
/** 
 * @defgroup Battle Battle
 * @ingroup Game
 * Everything involved for the "Battle" Portion of the game. 
 */

/** 
 * @defgroup Generator Generator
 * @ingroup Battle
 * The Generator portion of the CMB triad. 
 */

/** 
 * @addtogroup Generator
 * @{
 */

/** 
 * 
 * Generates a random number
 */
int genID()
{
  return rand() %1000;
}
/**
 * 
 * Generates a random boolean
 */
bool genBool()
{
  return (rand() %2);
}

/**
 * 
 * Generates a random string
 */
string genName()
{
  string s = "";
  int i,r;
  iter (i,0,5)
  {
    r = rand() %26 + 97;
    s.push_back (r);
  }
  return s;
}

/**
 * 
 * Prints state messages
 */
//buffer for the state message, length = length of buffer
void genStateMsg(char* buffer, int length, int time){
  switch ( (int) (rand() % 9))
  {
  case 0:
    snprintf(buffer, length, "%d /map %s",time, genName().c_str());
    cout << buffer << endl;
    if(debug) cout << buffer << endl;
    break;
  case 1:
    snprintf(buffer, length, "%d /zone %d %s %s %s %d",time, genID(), 
        genName().c_str(),genName().c_str(), genName().c_str(), genID());
   if(debug)cout << buffer << endl;
    break;
  case 2:
    snprintf(buffer,  length, "%d /user %d %s %d",time,  genID(), genName().c_str(), genID());
    if(debug)cout << buffer << endl;
    break;
  case 3:
    snprintf(buffer,  length, "%d /group %s %d",time, (genBool()) ? "ADD" : "DEL", genID());
    if(debug)cout << buffer<<endl;
    break;
  case 4:
    snprintf(buffer,  length, "%d /party %s %d",time, (genBool()) ? "INVITE":"JOIN", genID());
    if(debug)cout << buffer<<endl;
    break;
  case 5:
    snprintf(buffer,  length, "%d /fight %d %d",time, genID(), genID());
    if(debug)cout << buffer<<endl;
    break;
  case 6:
    snprintf(buffer,  length, "%d /start %d %d",time, genID(), genID());
    if(debug)cout << buffer<<endl;
    break;
  case 7:
    snprintf(buffer,  length, "%d /shoot %d %d %d",time, genID(), genID(), genID());
    if(debug)cout << buffer<<endl;
    break;
  default:
    snprintf(buffer,  length, "%d",time);
    if(debug)cout << buffer << endl;
  }
}

/**
 * @}
 */
