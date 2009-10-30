#include "GameObject.hpp"

namespace GameObject {

  void GameObj::movePhysics(int time, float gravity, float windspeed)
  {
    float deltat;
    deltat = (float) time;
    location.y = .5*gravity*deltat*deltat + yvelocity*deltat + location.y;
    location.x = .5*windspeed*deltat*deltat + xvelocity*deltat + location.x;
  }

  GameObj::GameObj(int objtype, Coord coord, float xv, float yv)
    : type(objtype), location(coord), xvelocity(xv), 
    yvelocity(yv){ };
}

/**
 * @}
 */
