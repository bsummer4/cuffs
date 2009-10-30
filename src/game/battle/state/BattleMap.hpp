#pragma once 
/**
 * @file
 * @author John R. Hoare
 */

#include <string>
#include <map>
#include <vector>
#include "coord.hpp"

/**
 * @defgroup BattleMap BattleMap
 * @ingroup State
 * @{
 */

/// Value pixel_type_t takes when the map is empty.
static const unsigned char MAP_EMPTY = 0;
/// Value pixel_type_t takes for a destructable pixel.
static const unsigned char MAP_DESTRUCTABLE = 128;
/// Value pixel_type_t takes for an destructable pixel.
static const unsigned char MAP_INDESTRUCTABLE = 255;

/// A typedef that is the container for one pixel for the map.
typedef unsigned char pixel_type_t;


/**
 * The Map class, this class holds a bitmap for the map.
 */
class BattleMap{
    public:
        BattleMap();
        ~BattleMap();
        void loadMap(std::string fileName);
        pixel_type_t getPixel(Coord c);
        pixel_type_t getPixel(int x, int y);
        void explosion(Coord c, float radius);
        void explosion(int x, int y, float radius);
        int getXSize();
        int getYSize();
        std::vector<Coord> getTeamSpawns(int team);

    private:
        void readPGM(std::string fileName);
    private:
        int x_size;
        int y_size;
        pixel_type_t * map;
        std::map< int , std::vector<Coord> > teamSpawnMap;
};

/**
 * @}
 */
