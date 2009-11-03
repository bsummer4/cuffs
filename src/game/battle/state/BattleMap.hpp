/**
 * @file
 * @author John R. Hoare
 */

#pragma once

#include <string>
#include <map>
#include <vector>
#include "coord.hpp"

/**
 * @defgroup BattleMap BattleMap
 * @ingroup State
 * @{
 */

/// @TODO Why can't doxygen infer this from the directory structure.
///       It's pretty damn obvious!

/// @TODO Maybe make an enum for these inside the class?

/// Value pixel_type_t takes when the map is empty.
static const unsigned char MAP_EMPTY = 255;
/// Value pixel_type_t takes for a destructable pixel.
static const unsigned char MAP_DESTRUCTABLE = 128;
/// Value pixel_type_t takes for an destructable pixel.
static const unsigned char MAP_INDESTRUCTABLE = 0;

/// A typedef that is the container for one pixel for the map.
typedef unsigned char pixel_type_t;


/**
 * The Map class, this class holds a bitmap for the map.

 * @TODO Consider making x_size, maxVal, y_size, map, and teamSpawnMap
 *       constant (and public, besides map).  Make the constructor
 *       take these things as arguments, and make loadMap static.
 *
 * @TODO Operator overloading [] might be appropriate here.
 */
class BattleMap {
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
    void outputMap(std::string outfile);

private:
    void readPGM(std::string fileName);
private:
    int x_size;
    int y_size;
    int maxVal;
    pixel_type_t * map;
    std::map< int , std::vector<Coord> > teamSpawnMap;
    bool is_destructable(pixel_type_t pixel);
    void destroy(pixel_type_t *pixel);
};

/**
 * @}
 */
