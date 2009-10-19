#pragma once 
#include <string>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include "State.hpp"
#include "GetCommand.hpp"
using namespace std;
using namespace GameUtils;

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
 * Interpreter virtual class.
 *
 */
class Interpreter {
    public:
        virtual void handleEvent(string &event) = 0;
};

/**
 * Cat Interpreter class. 
 *
 * The simple interpreter just prints out the message
 */
class CatInterpreter : public Interpreter{
    public: 
        CatInterpreter() {};
        virtual void handleEvent(string &event);
};

/**
 * Simple Interpreter class. 
 *
 * The simple interpreter just prints out the message
 */
class SimpleInterpreter : public Interpreter{
    public: 
        SimpleInterpreter();
        virtual void handleEvent(string &event);
};

/**
 * Game Interpreter. 
 *
 * Parses through the event message and modifies the game state. 
 */
class GameInterpreter : public Interpreter{
    public: 
        GameInterpreter(State &gameState);
        virtual void handleEvent(string &event);
    protected:
        State &state;
};

/**
 * @}
 */
