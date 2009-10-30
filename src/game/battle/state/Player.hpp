#pragma once 
#include <string>
#include "GameObject.hpp"
#include "coord.hpp"

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

class Player : public GameObject::GameObj {
  public:
    Player(int objId, std::string pname, Coord coord, int teamnum, int inithealth);
    int team;
    int health;
    void hit(int damage);
    std::string name;
};

/**
 * @}
 */
