#include "GameObject.hpp"

namespace GameObject {

  void GameObj::movePhysics(int time, float gravity, float windspeed)
  {
    float deltat;
    deltat = (float) time;
    location.y = .5*gravity*deltat*deltat + yvelocity*deltat + initlocation.y;
    location.x = .5*windspeed*deltat*deltat + xvelocity*deltat + initlocation.x;
  }

  GameObj::GameObj(int objID, int objtype, Coord coord, float xv, float yv)
    : id(objID), type(objtype), initlocation(coord), location(coord), xvelocity(xv), 
    yvelocity(yv){ };
}

/**
 * @}
 */
