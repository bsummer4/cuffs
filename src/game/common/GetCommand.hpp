#pragma once
#include <string>
#include <iostream>

namespace GameUtils {
enum { LOGIN, MAP, ZONE, USER, GROUP, PARTY, FIGHT, LAUNCHGAME, SHOOT, BATTLESTART, BATTLESTOP, WEAPON, MESSAGE, MOVE, HIT, QUERY};

int GetCommand(std::string command);
};
