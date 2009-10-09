#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <unistd.h>
using namespace std;
#define iter(var, from, to) for (int var=from; var<to; var++)

//Redefine this using inheritance
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


//Functions
//TODO
void initState(){
  state.append(vector<string> a(2));
  state[0].append("\\MAP");
  state[0].append(genName());
  state.append(vector<string> a(6));
  state[1].append("\\ZONE");
  state[1].append(genID());
  state[1].append(genName());
  state[1].append(genBool());
  state[1].append(genID());
  state.append(vector<string> a(6));
  }
  exit(1);
}

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
char* genStateMsg(char* buffer, int length){
  switch ( (int) (rand() % 9))
  {
  case 0:
    snprintf(buffer, size_t length, "/MAP %s\n", genName().c_str());
    break;
  case 1:
    snprintf(buffer, size_t length, "/MAP %s\n", genName().c_str());
    snprintf(buffer, size_t length, " /ZONE %d %s %s %s %d\n",genID(), genName().c_str(),
            genName().c_str(), genName().c_str(), genID());
    break;
  case 2:
    snprintf(buffer, size_t length, " /USER %d %s %d\n", genID(), genName().c_str(), genID());
    break;
  case 3:
    //cout << "/GROUP " << ( (genBool()) ? "ADD" : "DEL") << " " << genID() << endl;
    snprintf(buffer, size_t length, "/GROUP %s %s \n", (genBool()) ? "ADD" : "DEL", genID());
    break;
  case 4:
    snprintf(buffer, size_t length, "/PARTY %s %d\n", (genBool()) ? "INVITE":"JOIN", genID());
    break;
  case 5:
    snprintf(buffer, size_t length, " /FIGHT %d %d\n", genID(), genID());
    break;
  case 6:
    snprintf(buffer, size_t length, " /START %d %d\n", genID(), genID());
    break;
  case 7:
    snprintf(buffer, size_t length, " /SHOOT %d %d %d\n", genID(), genID(), genID());
    break;
  default:
    snprintf(buffer, size_t length, "\n");
  }
}


