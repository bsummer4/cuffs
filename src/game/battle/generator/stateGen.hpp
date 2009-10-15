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

/** 
 * @genid 
 * Generates a random number
 */
int genID()
{
  return rand() %1000;
}
/**
 * @genBool
 * Generates a random boolean
 */
bool genBool()
{
  return (rand() %2);
}

/**
 * @genName
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
 * @genStateMsg buffer length time
 * Prints state messages
 */
//buffer for the state message, length = length of buffer
void genStateMsg(char* buffer, int length, int time){
  switch ( (int) (rand() % 9))
  {
  case 0:
    snprintf(buffer, length, "%d /MAP %s\n",time, genName().c_str());
    cout << buffer << endl;
    if(debug) cout << buffer << endl;
    break;
  case 1:
    snprintf(buffer, length, "%d /MAP %s\n",time, genName().c_str());
    snprintf(buffer, length, "%d /ZONE %d %s %s %s %d\n",time, genID(), genName().c_str(),
            genName().c_str(), genName().c_str(), genID());
   if(debug)cout << buffer << endl;
    break;
  case 2:
    snprintf(buffer,  length, " %d /USER %d %s %d\n",time,  genID(), genName().c_str(), genID());
    if(debug)cout << buffer << endl;
    break;
  case 3:
    //cout << "/GROUP " << ( (genBool()) ? "ADD" : "DEL") << " " << genID() << endl;
    snprintf(buffer,  length, "%d /GROUP %s %d \n",time, (genBool()) ? "ADD" : "DEL", genID());
    if(debug)cout << buffer<<endl;
    break;
  case 4:
    snprintf(buffer,  length, "%d /PARTY %s %d\n",time, (genBool()) ? "INVITE":"JOIN", genID());
    if(debug)cout << buffer<<endl;
    break;
  case 5:
    snprintf(buffer,  length, "%d /FIGHT %d %d\n",time, genID(), genID());
    if(debug)cout << buffer<<endl;
    break;
  case 6:
    snprintf(buffer,  length, "%d /START %d %d\n",time, genID(), genID());
    if(debug)cout << buffer<<endl;
    break;
  case 7:
    snprintf(buffer,  length, "%d /SHOOT %d %d %d\n",time, genID(), genID(), genID());
    if(debug)cout << buffer<<endl;
    break;
  default:
    snprintf(buffer,  length, "%d\n",time);
    if(debug)cout << buffer << endl;
  }
}
