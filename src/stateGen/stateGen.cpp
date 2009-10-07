/*
Chris Craig
This code generates random states within the defined
protocol and prints to standard out.

Version 2
*/
/*
#include <time.h>
#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;

    * All Messages prepended by Timestamp
    * No slash = message display on console/screen

    * /MAP [name]
    * /ZONE ID OWNER CONTESTABLE BATTLE_ID
    * /USER ID NAME ADDRESS [-1 if not logged in]
    * /GROUP ADD ID USERID
    * /GROUP DEL ID USERID
    * /PARTY INVITE ID USERID
    * /PARTY JOIN ID USERID
    * /FIGHT PARTYID ZONE ID
    * /START TURN PLAYERID
    * /SHOOT ANGLE POWER WEAPONID
*/
#include "stateGen.hpp"
/*
//Useless really
int time_diff(long end, long now){
  return end-now;
}
*/

//Code ripped straight from "C++:Reference"
void wait (int seconds)
{
  clock_t endwait;
  endwait = clock () + seconds * CLOCKS_PER_SEC ;
  while (clock() < endwait) {} };

int genID()
{
  return rand() %1000;
}
bool genBool()
{
  return (rand() %2);
}

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

void genStateMsg()
{
  switch ( (int) (rand() % 10))
  {
  case 0:
    printf (" /MAP %s\n", genName().c_str());
    break;
  case 1:
    printf (" /ZONE %d %s %s %s %d\n",genID(), genName().c_str(),
            genName().c_str(), genName().c_str(), genID());
    break;
  case 2:
    printf (" /USER %d %s %d\n", genID(), genName().c_str(), genID());
    break;
  case 3:
    cout << "/GROUP " << ( (genBool()) ? "ADD" : "DEL") << " " << genID() << endl;
    break;
  case 4:
    printf (" /PARTY %s %d\n", (genBool()) ? "INVITE":"JOIN", genID());
    break;
  case 5:
    printf (" /FIGHT %d %d\n", genID(), genID());
    break;
  case 6:
    printf (" /START %d %d\n", genID(), genID());
    break;
  case 7:
    printf (" /SHOOT %d %d %d\n", genID(), genID(), genID());
    break;
  default:
    printf ("\n");
  }
}

//TODO What defines a "Timestamp"
//ANSWER: for now, its just rawtime
int main (int argc, char **argv)
{
  time_t rawtime;
  time (&rawtime);
  srand (time (NULL));

  for (int ii=0; ii<9; ii++)
  {
    printf ("%d ", (int) rawtime);
    genStateMsg();
    fflush (stdin);
  }

  return 0;
}
