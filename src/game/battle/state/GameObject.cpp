#include "GameObject.hpp"
using namespace std;

namespace GameObject {

Coord GameObj::movePhysics(int time, float gravity, float windspeed) {
  Coord returnval;
  float deltat;
  deltat = (float) time;
  returnval.y = floor(.5*gravity*deltat*deltat + yvelocity*deltat + initlocation.y + .5);
  returnval.x = floor(.5*windspeed*deltat*deltat + xvelocity*deltat + initlocation.x + .5);
  return returnval;
}

GameObj::GameObj(std::string objID, int objtype, Coord coord, float xv, float yv)
  : id(objID), type(objtype), initlocation(coord), location(coord), xvelocity(xv),
  yvelocity(yv) { };
}

/**
 * @}
 */
