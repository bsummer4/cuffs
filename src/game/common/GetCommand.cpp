#include "GetCommand.hpp"
using namespace std;

namespace GameUtils{
  int GetCommand(string command) {
    if (command.compare("/MAP") == 0) {
      return MAP;
    } else if (command.compare("/SHOOT") == 0) {
      return SHOOT;
    } else if (command.compare("/GAMESTART") == 0) {
      return GAMESTART;
    } else if (command.compare("/GAMESTOP") == 0) {
      return GAMESTOP;
    }
  }
}
