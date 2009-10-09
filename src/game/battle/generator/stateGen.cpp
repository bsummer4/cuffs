/*
Chris Craig
This code generates random states within the defined
protocol and prints to standard out.

Version 2
*/
/*
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
#include "switchbox_client.h"
#pragma once MAXLEN=512
//vector< vector<string> > state(8); //the global variable for states

//TODO What defines a "Timestamp"
//ANSWER: for now, its just rawtime
int startGen()
{
  time_t rawtime;
  srand (time (NULL));
  SBMessage* message = NULL;
  char buffer[MAXLEN];
  for (;;)
  {
    //TODO generate a connection object and go
    time (&rawtime);
    printf ("%d ", (int) rawtime);
    genStateMsg(buffer, MAXLEN);
    message = string_to_message(BROADCAST, 0, 0, buffer);
    fflush (stdin);
    sleep(1);
  }
  return 0;
}
