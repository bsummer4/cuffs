#include "BattleMap.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

BattleMap::BattleMap(){ }

BattleMap::~BattleMap(){
    delete map;
}

/**
 * Load in the map from the given file name.
 * 
 * A map file specifies the spawn points first, and 
 * then the map file (which is a ppm file). The map file 
 * starts by listing the spawns with TeamId Xpos Ypos seperated
 * with spaces, and continues until all spawn positions are
 * enumerated. Once this occurs, a string that contains the name
 * of the .ppm file to load up as a map is given. 
 * 
 * @param fileName fileName The name of the .map file to open. 
 */
void BattleMap::loadMap(std::string fileName){
  ifstream inFile(fileName.c_str());
  int team, xpos, ypos;
  char inputLine1[81];
  int val = 0; 
  std::map< int , std::vector<Coord> >::iterator it;
  inFile.getline(inputLine1,80);

  val = sscanf(inputLine1, "%d %d %d", &team, &xpos, &ypos);
  if ( val == 3 ){
    it = teamSpawnMap.find(team);
    if( it == teamSpawnMap.end() ){
      teamSpawnMap.insert( pair < int , std::vector<Coord> > (team, std::vector<Coord>() ) );
      it = teamSpawnMap.find(team);
    }
    (*it).second.push_back(Coord(xpos,ypos));
  } else {
    // If sscanf fails, assume that we're done and this is the map file
    readPGM(inputLine1);
  }
}

/**
 * A helper function for @ref loadMap that creates 
 * the map data structure. 
 * 
 * @param fileName The name of the .ppm file. 
 */
void BattleMap::readPGM(std::string fileName){
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
            if ( nextChar == MAP_EMPTY ||
                 nextChar == MAP_DESTRUCTABLE ||
                 nextChar == MAP_INDESTRUCTABLE ) {
                map[i*x_size+j] = nextChar;
            } else { 
                cerr << "Got a bad pixel value in the image" << endl;
                ///@TODO Throw an exception here.
            }
        }
    }
    cout << "Map input complete.\n";
}

/**
 * @param x The x value 
 * @param y The y value
 * @return The value of the pixel at the given point.
 */
pixel_type_t BattleMap::getPixel(int x, int y){
    return map[x*x_size+y];
}

/**
 * @param c The coordinate object describing the point 
 * that you want the pixel information of.
 * @return The value of the pixel at the given point.
 */
pixel_type_t BattleMap::getPixel(Coord c){
    return map[c.x*x_size+c.y];
}

/**
 * Get a vector of all the spawn points for the given 
 * team number
 */
std::vector<Coord> BattleMap::getTeamSpawns(int team){
    std::map< int , std::vector<Coord> >::iterator it;
    it = teamSpawnMap.find(team);
    if ( it != teamSpawnMap.end() ){
        return std::vector<Coord>();
    }
    return (*it).second;
}

/** 
 * Removes all terrain within radius of x,y
 */
void BattleMap::explosion(int x, int y, float radius){
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

/**
 * Wrapper for @ref explosion that uses a Coord 
 * object instead of integer parameters.
 */
void BattleMap::explosion(Coord c, float radius){
    return explosion(c.x, c.y, radius);
}
