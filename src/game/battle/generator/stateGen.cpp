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
#define MAXLEN 512 

//vector< vector<string> > state(8); //the global variable for states

//TODO Read timestamps from Sync
//ANSWER: for now, its just rawtime
int main(int argc, char** argv)
{
  if(argc >1){
  time_t rawtime;
//  debug = (argv[1] == "-D");//Please dont spit errors
  srand (time (NULL));
  SBMessage* message = NULL;
  string hostname("localhost");
  Connection c = Connection(hostname.c_str(), 80044);
  char buffer[MAXLEN];
  for (;;)
  {
    time (&rawtime);
    printf ("%d ", (int)rawtime);
    genStateMsg(buffer, MAXLEN);
    c.sendMessage(string_to_message(BROADCAST, 0, 0, buffer));
//fflush (stdin);
    sleep(1);
  }
  return 0;
}
