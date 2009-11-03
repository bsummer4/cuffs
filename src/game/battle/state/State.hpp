#pragma once
#include <string>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Projectile.hpp"
#include "GameObject.hpp"
#include "Player.hpp"
#include "BattleMap.hpp"
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
 * The State portion of the CMB triad.
 */

/**
 * @addtogroup State
 * @{
 */

/**
 * State virtual class.
 *
 */

static const int DEBUG = 1;

class State {
public:
    State();
    // RETURNS NEW SPEED, IF CHANGED
    virtual double changeWind(float newwind);
    // RETURNS SPEED
    virtual double getWind();
    virtual void projHit(Coord coord, Projectile &proj);
    // Gets the pixel defined at row, column
    virtual pixel_type_t getPixel(Coord coord);
    // Gets the pixel defined at row, column
    virtual pixel_type_t getPixel(int x, int y);
    // Not fully implemented yet
    virtual std::string setMap(std::string mapname);
    // Not fully implemented yet
    virtual std::string getMapName();
    // Not fully implemented yet
    virtual void startBattle();
    // Not fully implemented yet
    virtual void stopBattle();
    virtual bool getBattleState();
    // Sets current weapon
    virtual bool changeWeapon(std::string weapon);
    // Returns current weapon
    virtual std::string getWeapon();
    virtual void addPlayer(std::string name, Coord coord, int team, int health);
    virtual void addProjectile(std::string projID, std::string weapontype,
                               Coord coord, float xvel, float yvel);
    virtual void addProjectile(std::string projID, std::string weapontype,
                               int x, int y, float xvel, float yvel);
    virtual float getGravity();
    virtual void setGravity(float newgrav);
    virtual void moveObj(std::string obj_id, int x, int y);
    virtual void hitObj(std::string obj_id, int x, int y);
    virtual Coord getPlayerLocation(std::string userid);
    virtual std::string newProjID();

protected:
    virtual void setupMap();
    std::string userid;
    // wind speed and direction--negative for left positive for right
    float wind;
    std::ifstream mapstream;
    std::string mapname;
    bool battlestarted;
    std::string weaponid;
    std::map<std::string, Player *> players;
    std::map<std::string, Projectile *> projectiles;
    std::map<std::string, Projectile *> myprojectiles;
    std::map<std::string, GameObject::GameObj *> objects;
    BattleMap bmap;
    float gravity;
    int projnumber;
};

/**
 * @}
 */
