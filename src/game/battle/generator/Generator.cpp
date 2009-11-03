/*
Chris Craig

Version 2
*/
/*
 * All Messages prepended by Timestamp:
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

/**
  * Generator Constructor
  * Builds the Generator class
  *
**/
/* Code goes here*/

/**
 * genid
 * Generates a random number
 */
int Generator::genID()
{
    return rand() %1000;
};
/**
 * genBool
 * Generates a random boolean
 */
bool Generator::genBool()
{
    return (rand() %2);
}

/**
 * genName
 * Generates a random string
 */
string Generator::genName()
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
 * genStateMsg buffer length time
 * Prints state messages
 */
//buffer for the state message, length = length of buffer
void Generator::genStateMsg(char* buffer, int length, int time) {
    time++;
    switch ( (int) (rand() % 9))
    {
    case 0:
        snprintf(buffer, length, "%d /MAP %s",time, genName().c_str());
        if (debug) cout << buffer << endl;
        break;
    case 1:
        snprintf(buffer, length, "%d /ZONE %d %s %s %s %d",time, genID(),
                 genName().c_str(),genName().c_str(), genName().c_str(), genID());
        if (debug)cout << buffer << endl;
        break;
    case 2:
        snprintf(buffer,  length, "%d /USER %d %s %d",time,  genID(), genName().c_str(), genID());
        if (debug)cout << buffer << endl;
        break;
    case 3:
        snprintf(buffer,  length, "%d /GROUP %s %d",time, (genBool()) ? "ADD" : "DEL", genID());
        if (debug)cout << buffer<<endl;
        break;
    case 4:
        snprintf(buffer,  length, "%d /PARTY %s %d",time, (genBool()) ? "INVITE":"JOIN", genID());
        if (debug)cout << buffer<<endl;
        break;
    case 5:
        snprintf(buffer,  length, "%d /FIGHT %d %d",time, genID(), genID());
        if (debug)cout << buffer<<endl;
        break;
    case 6:
        snprintf(buffer,  length, "%d /START %d %d",time, genID(), genID());
        if (debug)cout << buffer<<endl;
        break;
    case 7:
        snprintf(buffer,  length, "%d /SHOOT %d %d %d",time, genID(), genID(), genID());
        if (debug)cout << buffer<<endl;
        break;
    default:
        snprintf(buffer,  length, "%d",time);
        if (debug)cout << buffer << endl;
    }
}

/**
 * @}
 */
