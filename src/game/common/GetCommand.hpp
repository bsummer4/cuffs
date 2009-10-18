#include <string>
using namespace std;

namespace GameUtils {
  enum { LOGIN, MAP, ZONE, USER, GROUP, PARTY, FIGHT, LAUNCHGAME, SHOOT, BATTLESTART, BATTLESTOP, WEAPON };

  int GetCommand(string command);
};
