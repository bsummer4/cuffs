#pragma once 
#include <string>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "BattleMap.hpp"
#include "coord.hpp"
#include "GameObject.hpp"
#include "Projectile.hpp"
#include "Player.hpp"
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

static const int DEBUG = 1;

class State {
    public:
        State();
        virtual double changeWind(float newwind); //RETURNS NEW SPEED, IF CHANGED
        virtual double getWind(); //RETURNS SPEED
        virtual void hitObj(Coord coord, Projectile &proj);
        virtual pixel_type_t getPixel(Coord coord);  //Gets the pixel defined at row, column
        virtual string setMap(string mapname);//Not fully implemented yet
        virtual string getMapName();//Not fully implemented yet
        virtual void startBattle(); //Not fully implemented yet
        virtual void stopBattle();  //Not fully implemented yet
        virtual bool getBattleState();
        virtual bool changeWeapon(int weapon);  //sets current weapon
        virtual int getWeapon();    //returns current weapon
        virtual void addPlayer(Coord coord, int team, int health);
        virtual void addProjectile(int weapontype, Coord coord, float xvel, float yvel);
        virtual float getGravity();
        virtual void setGravity(float newgrav);

    protected:
        virtual void setupMap();
        float wind;  //wind speed and direction--negative for left positive for right
        ifstream mapstream;
        string mapname;
        bool battlestarted;
        int weaponid;
        vector<Player> players;
        vector<Projectile> projectiles;
        BattleMap map;
        float gravity;
};
/**
 * @}
 */
