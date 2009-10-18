#pragma once 
#include <string>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
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
 * The State portion of the CMB triad. 
 */

/** 
 * @addtogroup State
 * @{
 */

///MAP PIXEL DEFINED AS
#define MAPPIXEL int


/**
 * State virtual class.
 *
 */

class State {
    public:
        state();
        virtual double changeWind(double newwind); //RETURNS NEW SPEED, IF CHANGED
        virtual double getWind(); //RETURNS SPEED
        virtual void changePixel(int row, int column, MAPPIXEL newpixel);
        virtual MAPPIXEL getPixel(int row, int column);  //Gets the pixel defined at row, column
        virtual bool setMap(string mapname);
        virtual bool getMap();
    protected:
        double wind;  //wind speed and direction--negative for left positive for right
        vector< vector< MAPPIXEL > > map;
        ifstream map;
};

/**
 * @}
 */
