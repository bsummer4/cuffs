#pragma once
#include <string>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include "State.hpp"
#include "GetCommand.hpp"
#include "coord.hpp"
#include <cmath>

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
  virtual void handleEvent(std::string &event) = 0;
};

/**
 * Cat Interpreter class.
 *
 * The simple interpreter just prints out the message
 */
class CatInterpreter : public Interpreter {
public:
  CatInterpreter() {};
  virtual void handleEvent(std::string &event);
};

/**
 * Simple Interpreter class.
 *
 * The simple interpreter just prints out the message
 */
class SimpleInterpreter : public Interpreter {
public:
  SimpleInterpreter();
  virtual void handleEvent(std::string &event);
};

/**
 * Game Interpreter.
 *
 * Parses through the event message and modifies the game state.
 */
class GameInterpreter : public Interpreter {
public:
  GameInterpreter(State &gameState);
  virtual void handleEvent(std::string &event);
protected:
  State &state;
};

/**
 * @}
 */
