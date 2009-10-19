#pragma once 
#include <string>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
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
        State();
        //virtual double changeWind(double newwind); //RETURNS NEW SPEED, IF CHANGED
        //virtual double getWind(); //RETURNS SPEED
        //virtual void changePixel(int row, int column, MAPPIXEL newpixel);
        //virtual MAPPIXEL getPixel(int row, int column);  //Gets the pixel defined at row, column
        virtual string setMap(string mapname);//Not fully implemented yet
        virtual string getMapName();//Not fully implemented yet
        virtual void startBattle(); //Not fully implemented yet
        virtual void stopBattle();  //Not fully implemented yet
        virtual bool getBattleState();
        virtual bool changeWeapon(int weapon);  //sets current weapon
        virtual int getWeapon();    //returns current weapon
        vector< vector< MAPPIXEL > > map; //READ ONLY

    protected:
        virtual void setupMap();
        //double wind;  //wind speed and direction--negative for left positive for right
        ifstream mapstream;
        string mapname;
        int battlestarted;
        int weaponid;
};

/**
 * @}
 */
