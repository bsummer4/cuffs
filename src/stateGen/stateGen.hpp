#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
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
  int POWER;
  int WEAPON_ID;
};
