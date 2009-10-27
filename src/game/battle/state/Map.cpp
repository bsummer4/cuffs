#include "Map.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

Map::Map(){ }

Map::~Map(){
    delete map;
}

/**
 * Load in the map from the given file name.
 * 
 * @TODO implement bringing in spawn points as well.
 */
void Map::loadMap(std::string fileName){
    ifstream inFile(fileName.c_str());

    char inputLine1[81];
    char nextChar;
    int maxVal;

    /* Read past first line */
    inFile.getline(inputLine1,80);

    /* Read in width, height, maxVal */
    inFile >> x_size >> y_size >> maxVal;
    cout << "Width = " << x_size << ", Height = " << y_size << endl;

    map = new pixel_type_t[x_size*y_size];

    for (int i = 0; i< x_size*y_size; i++) map[i] = 0;

    /* Read in map; */
    for (int i=0; i<x_size; i++){
        for (int j=0; j<y_size; j++) {
            inFile >> nextChar;
            map[i*x_size+j] = nextChar;
        }
    }
    cout << "Map input complete.\n";
}

/*
 * @return The value of the pixel at the given point.
 */
pixel_type_t Map::getPixel(int x, int y){
    return map[x*x_size+y];
}

/*
 * @return The value of the pixel at the given point.
 */
pixel_type_t Map::getPixel(Coord c){
    return map[c.x*x_size+c.y];
}

/** 
 * Removes all terrain within radius of x,y
 */
void Map::explosion(int x, int y, float radius){
    int s_x, e_x, s_y, e_y;
    s_x = max(0,(int)(x-ceil(radius)));
    e_x = min(x_size,(int)(x+ceil(radius)));
    s_y = max(0,(int)(y-ceil(radius)));
    s_x = min(y_size,(int)(y+ceil(radius)));

    for (int myx = s_x; myx < e_x; myx++){
        for (int myy = s_y; myy < e_y; myy++){
            if ( hypot(myx-x,myy-y) < radius ){
                map[myx*x_size+myy] = MAP_EMPTY;
            }
        }
    }
}

void Map::explosion(Coord c, float radius){
    return explosion(c.x, c.y, radius);
}
