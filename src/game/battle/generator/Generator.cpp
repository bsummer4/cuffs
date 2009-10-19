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

#include "Generator.hpp"
#define MAXLEN 512 

///@TODO Read timestamps from Sync
///ANSWER: for now, its just rawtime
int main(int argc, char** argv)
{
  if(argc == 2){
    string deb(argv[1]);
    debug = (deb == "-D" || deb == "-d");//Please dont spit errors
  }
  time_t rawtime;
  srand(time (NULL));
  SBMessage* message = NULL;
  string hostname("localhost");
  Connection c = Connection(hostname.c_str(), 80044);
  char buffer[MAXLEN];
  for (;;)
  {
    time (&rawtime);
    genStateMsg(buffer, MAXLEN, (int)rawtime);
    c.sendMessage(string_to_message(BROADCAST, 0, 0, buffer));
    sleep(1); //Assumed it will wait 1/2 a sec untill the next message
  }
  return 0;
}
