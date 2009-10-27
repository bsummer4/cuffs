#pragma once 
/*
 * @file
 * @author John R. Hoare
 */

typdef unsigned int pixel_type_t;

class Map{
    public:
        Map();
        void loadMap(std::string fileName);
        pixel_type_t getPixel(int x, int y);
        void explosion(int x, int y, float radius);
};
