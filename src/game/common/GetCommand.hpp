#include <string>
using namespace std;

namespace GameUtils {
  enum { LOGIN, MAP, ZONE, USER, GROUP, PARTY, FIGHT, LAUNCHGAME, SHOOT, GAMESTART, GAMESTOP };

  int GetCommand(string command);
};
