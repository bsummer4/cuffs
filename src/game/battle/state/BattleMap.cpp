#include "BattleMap.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>

#define POINT(X,Y) x_size*(Y)+(X)

using namespace std;

BattleMap::BattleMap() { }

BattleMap::~BattleMap() {
  delete map;
}

/**
 * Load in the map from the given file name.
 *
 * A map file specifies the spawn points first, and then the map file
 * (which is a ppm file). The map file starts by listing the spawns
 * with TeamId Xpos Ypos seperated with spaces, and continues until
 * all spawn positions are enumerated. Once this occurs, a string that
 * contains the name of the .ppm file to load up as a map is given.
 *
 * @TODO Do a lot more error checking here. This is assuming it has a
 *       good file.
 *
 * @TODO The is no reason to expect lines in the map to have fixed
 * lengh.
 *
 * @param fileName The name of the .map file to open.
 */
void BattleMap::loadMap(std::string fileName) {
  ifstream inFile(fileName.c_str());
  int team, xpos, ypos;
  char inputLine1[81];
  int val = 0;
  std::map< int , std::vector<Coord> >::iterator it;

  while(1) {
    inFile.getline(inputLine1,80);
    //cout << "Line: " << inputLine1 << endl;
    val = sscanf(inputLine1, "%d %d %d", &team, &xpos, &ypos);
    if(val == 3) {
      it = teamSpawnMap.find(team);
      if(it == teamSpawnMap.end()) {
        teamSpawnMap.insert(pair < int , std::vector<Coord> > (team, std::vector<Coord>()));
        it = teamSpawnMap.find(team);
      }
      (*it).second.push_back(Coord(xpos,ypos));
    } else {
      // If sscanf fails, assume that we're done and this is the map file
      readPGM(inputLine1);
      return;
    }
  }
}

/**
 * A helper function for @ref loadMap that creates
 * the map data structure.
 *
 * @param fileName The name of the .ppm file.
 */
void BattleMap::readPGM(std::string fileName) {
  ifstream inFile(fileName.c_str());
  cout << "Reading in " << fileName << endl;

  char inputLine1[81];
  unsigned int nextChar;
  /* Read past first line */
  inFile.getline(inputLine1,80);

  /* Read in width, height, maxVal */
  inFile >> x_size >> y_size >> maxVal;
  //cout << "Width = " << x_size << ", Height = " << y_size << endl;

  map = new pixel_type_t[x_size*y_size];

  for(int i = 0; i< x_size*y_size; i++) map[i] = 0;

  /* Read in map; */
  for(int y=0; y < y_size; y++) {
    for(int x=0; x < x_size; x++) {
      inFile >> nextChar;
      if(nextChar == MAP_EMPTY ||
          nextChar == MAP_DESTRUCTABLE ||
          nextChar == MAP_INDESTRUCTABLE) {
        // y_size - j to flip the map around to put origin in bottom left.
        map[POINT(x,(y_size-y-1))] = nextChar;
        //map[POINT(x,y)] = nextChar;
      } else {
        cerr << "Got a bad pixel value in the image: " << (int)nextChar << endl;
        ///@TODO Throw an exception here.
      }
    }
  }
  //cout << "Map input complete.\n";
}

/**
 * A function that is there just for testing purposes.
 * maybe it will be used for something else later.
 *
 * Saves the current state of the map into a pgm file.
 *
 */
void BattleMap::outputMap(std::string outfile) {
  ofstream outFile(outfile.c_str());
  outFile << "P5" << endl;
  outFile << x_size << " " << y_size << endl
          << maxVal << endl;

  for(int j=0; j<y_size; j++) {
    for(int i=0; i<x_size; i++) {
      //outFile << map[(y_size-j-1)*x_size+i];
      //outFile << map[POINT(i,(y_size-j-1))];
      //outFile << map[POINT(i,j)];
      outFile << map[POINT(i,(y_size-j-1))];
    }
  }
  //cout << "Scaled map output to file.\n";
}

/**
 * @param x The x value
 * @param y The y value
 * @return The value of the pixel at the given point.
 */
pixel_type_t BattleMap::getPixel(int x, int y) {
  //cout << "x,y = (" << x << "," << y << ") = " << POINT(x,y) << " = " << (int)map[POINT(x,y)] << endl;
  return map[POINT(x,y)];
}

/**
 * @param c The coordinate object describing the point
 * that you want the pixel information of.
 * @return The value of the pixel at the given point.
 */
pixel_type_t BattleMap::getPixel(Coord c) {
  return map[POINT(c.x,c.y)];
}

/**
 * Get a vector of all the spawn points for the given
 * team number
 */
std::vector<Coord> BattleMap::getTeamSpawns(int team) {
  std::map< int , std::vector<Coord> >::iterator it;
  it = teamSpawnMap.find(team);
  if(it == teamSpawnMap.end()) {
    return std::vector<Coord>();
  }
  return (*it).second;
}

bool BattleMap::is_destructable(pixel_type_t pixel) {
  return pixel == MAP_DESTRUCTABLE;
}

void BattleMap::destroy(pixel_type_t *pixel) {
  *pixel = MAP_EMPTY;
}


/**
 * Removes all destructable terrain within radius of x,y
 *
 * @TODO This actually deletes a square for now.  It's much easier to
 * implement.  For now, let's just pretend that's what we really want.
 */
void BattleMap::explosion(int center_x, int center_y, float radius) {
  if(radius <= 0) return;
  int r = ceil(radius);
  int low_x = max(0, center_x - r);
  int high_x = min(x_size - 1, center_x + r);
  int low_y = max(0, center_y - r);
  int high_y = min(y_size - 1, center_y + r);

  for(int x = low_x; x <= high_x; x++)
    for(int y = low_y; y <= high_y; y++) {
      pixel_type_t *pixel = map + POINT(x,y);
      // To destroy a circle, check if:
      //   distance((x, y), (center_x, center_y)) <= r
      if(is_destructable(*pixel))
        destroy(pixel);
    }
}

/// Wrapper for @ref explosion.
void BattleMap::explosion(Coord c, float radius) {
  return explosion(c.x, c.y, radius);
}


/// Return the size of the map in the x direction in pixels.
int BattleMap::getXSize() {
  return x_size;
}

/// Return the size of the map in the y direction in pixels.
int BattleMap::getYSize() {
  return y_size;
}
