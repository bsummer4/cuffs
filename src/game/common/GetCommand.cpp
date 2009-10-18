#include "GetCommand.hpp"
using namespace std;

namespace GameUtils{

  ///RETURNS THE ENUMERATED INTEGER ASSOCIATED WITH EACH COMMAND
  int GetCommand(string command) {
    if (command.compare("/map") == 0) {
      return MAP;
    } else if (command.compare("/shoot") == 0) {
      return SHOOT;
    } else if (command.compare("/battlestart") == 0) {
      return BATTLESTART;
    } else if (command.compare("/battlestop") == 0) {
      return BATTLESTOP;
    } else if (command.compare("/weapon") == 0) {
      return WEAPON;
    } else if (command.compare("/login") == 0) {
      return LOGIN;
    } else if (command.compare("/zone") == 0) {
      return ZONE;
    } else if (command.compare("/user") == 0) {
      return USER;
    } else if (command.compare("/group") == 0) {
      return GROUP;
    } else if (command.compare("/party") == 0) {
      return PARTY;
    } else if (command.compare("/fight") == 0) {
      return FIGHT;
    } else if (command.compare("/launchgame") == 0) {
      return LAUNCHGAME;
    }
  }
}
