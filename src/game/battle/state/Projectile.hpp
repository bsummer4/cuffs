#pragma once 
#include <string>
#include "GameObject.hpp"
#include "coord.hpp"
using namespace std;
using namespace GameObject;

/** 
 * @defgroup Game Game
 * Everything that goes into the game backend. 
 */
/** 
 * @defgroup Battle Battle
 * @ingroup Game
 * Everything involved for the "Battle" Portion of the game. 
 */

/** 
 * @defgroup State State
 * @ingroup Battle
 * State Interface
 */

/** 
 * @defgroup GameObject GameObject
 * @ingroup State
 * GameObject Interface
 */

/** 
 * @defgroup Projectile Projectile
 * @ingroup GameObject
 * Projectile Class
 */

/** 
 * @addtogroup GameObject
 * @{
 */

/**
 * Projectile class.
 *
 */

class Projectile : public GameObj {
  public:
    Projectile(int type, Coord coord, float rad);
    Projectile(int type, int x, int y, float rad);
    float radius;
};

/**
 * @}
 */
