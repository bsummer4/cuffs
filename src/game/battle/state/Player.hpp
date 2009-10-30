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
 * @defgroup Player Player
 * @ingroup GameObject
 * Player Class
 */

/** 
 * @addtogroup GameObject
 * @{
 */

/**
 * Player class.
 *
 */

class Player : public GameObj {
  public:
    Player(Coord coord, int teamnum, int inithealth);
    int team;
    int health;
    void hit(int damage);
};

/**
 * @}
 */
