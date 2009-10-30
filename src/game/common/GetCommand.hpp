#pragma once
#include <string>
#include <iostream>
using namespace std;

namespace GameUtils {
  enum { LOGIN, MAP, ZONE, USER, GROUP, PARTY, FIGHT, LAUNCHGAME, SHOOT, BATTLESTART, BATTLESTOP, WEAPON, MESSAGE, MOVE};

  int GetCommand(string command);
};
