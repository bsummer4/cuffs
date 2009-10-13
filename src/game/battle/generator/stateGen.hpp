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

//Redefine this using inheritance
/*
class STATE
{
  string name;
};
class MAP
{
  string name;
};
class ZONE
{
  int ID;
  string NAME;
  string OWNER;
  bool CONTESTABLE;
  int BATTLE_ID;
};
class USER
{
  int ID;
  string NAME;
  int ADDRESS;
};
class GROUP
{
  string ACTION; //either ADD or DEL
  int USER_ID;
};
class PARTY
{
  string ACTION; //either INVITE or JOIN
  int GROUP_ID;
  int USER_ID;
};
class FIGHT
{
  int PARTY_ID;
  int ZONE_ID;
};
class START
{
  int TURN;
  int PLAYER_ID;
};
class SHOOT
{
  int ANGLE;
  int WEAPON_ID;
};
*/

/*
 *Functions: For your viewing pleasure
 */
//Generates a random number
int genID()
{
  return rand() %1000;
}
//generates a random boolean
bool genBool()
{
  return (rand() %2);
}
//generates a random name
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

//Prints state messages
//buffer for the state message, length = length of buffer
void genStateMsg(char* buffer, int length){
  switch ( (int) (rand() % 9))
  {
  case 0:
    snprintf(buffer, length, "/MAP %s\n", genName().c_str());
    if(debug) cout << buffer;
    break;
  case 1:
    snprintf(buffer, length, "/MAP %s\n", genName().c_str());
    snprintf(buffer, length, " /ZONE %d %s %s %s %d\n",genID(), genName().c_str(),
            genName().c_str(), genName().c_str(), genID());
    if(debug)cout << buffer;
    break;
  case 2:
    snprintf(buffer,  length, " /USER %d %s %d\n", genID(), genName().c_str(), genID());
    if(debug)cout << buffer;
    break;
  case 3:
    //cout << "/GROUP " << ( (genBool()) ? "ADD" : "DEL") << " " << genID() << endl;
    snprintf(buffer,  length, "/GROUP %s %d \n", (genBool()) ? "ADD" : "DEL", genID());
    if(debug)cout << buffer;
    break;
  case 4:
    snprintf(buffer,  length, "/PARTY %s %d\n", (genBool()) ? "INVITE":"JOIN", genID());
    if(debug)cout << buffer;
    break;
  case 5:
    snprintf(buffer,  length, " /FIGHT %d %d\n", genID(), genID());
    if(debug)cout << buffer;
    break;
  case 6:
    snprintf(buffer,  length, " /START %d %d\n", genID(), genID());
    if(debug)cout << buffer;
    break;
  case 7:
    snprintf(buffer,  length, " /SHOOT %d %d %d\n", genID(), genID(), genID());
    if(debug)cout << buffer;
    break;
  default:
    snprintf(buffer,  length, "\n");
    if(debug)cout << buffer;
  }
}
