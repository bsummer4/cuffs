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

class Projectile : public GameObject::GameObj {
public:
    Projectile(std::string objID, std::string type, Coord coord, float xv, float yv);
    std::string weapontype;
    float radius;
    int id;
};

/**
 * @}
 */
