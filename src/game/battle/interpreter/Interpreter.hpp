#pragma once 

#include<string>

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
 * @defgroup Interpreter Interpreter
 * @ingroup Battle
 * The Interpreter portion of the CMB triad. 
 */

/** 
 * @addtogroup Interpreter
 * @{
 */


/**
 * Listener virtual class.
 *
 */
class Listener {
    virtual void HandleEvent(std::string event) = 0;
};

/**
 * Simple Interpreter class. 
 *
 * The simple interpreter just prints out the message
 */
class SimpleInterpreter : public Listener{
    public: 
        SimpleInterpreter();
        virtual void HandleEvent(std::string event);
};

/**
 * Game Interpreter. 
 *
 * Parses through the event message and modifies the game state. 
 */
class GameInterpreter : public Listener{
    public: 
        GameInterpreter();
        virtual void HandleEvent(std::string event);
};

/**
 * @}
 */
