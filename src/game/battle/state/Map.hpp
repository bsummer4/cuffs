#pragma once 
/**
 * @file
 * @author John R. Hoare
 */

#include <string>
#include "coord.hpp"

static const char MAP_EMPTY = 0;

typedef unsigned char pixel_type_t;

/**
 * The Map class, this class holds a bitmap for the map.
 */
class Map{
    public:
        Map();
        ~Map();
        void loadMap(std::string fileName);
        pixel_type_t getPixel(Coord c);
        pixel_type_t getPixel(int x, int y);
        void explosion(Coord c, float radius);
        void explosion(int x, int y, float radius);
        int getXSize();
        int getYSize();
    private:
        int x_size;
        int y_size;
        pixel_type_t * map;
};
