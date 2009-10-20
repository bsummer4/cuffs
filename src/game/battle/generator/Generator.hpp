#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <unistd.h>
extern "C" {
#include "message.h"
#include "switchbox_client.h"
}
#include "Connection.hpp"
using namespace std;

//Global flag debug
bool debug;

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
 * @defgroup Generator Generator
 * @ingroup Battle
 * The Generator portion of the CMB triad. 
 */

/** 
 * @addtogroup Generator
 * @{
 */

/** 
 * Generator Class
 * 
=======
 * 
 * Generates a random number
 */
class Generator { 
  public:
    int genID();
    bool genBool();
    string genName();
    void genStateMsg(char* buffer, int length, int time);
    int genRun(); 
}; 
/**
 * @}
 */
