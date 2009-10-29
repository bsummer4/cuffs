#pragma once 
#include <string>
#include "coord.hpp"
using namespace std;

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
 * @addtogroup State
 * @{
 */

/**
 * GameObject virtual class.
 *
 */

namespace GameObject {

  enum { PLAYER, PROJECTILE };

  class GameObj {
    public:
      Coord location;
      int type;
    protected:
      GameObj();
  };

}
/**
 * @}
 */
