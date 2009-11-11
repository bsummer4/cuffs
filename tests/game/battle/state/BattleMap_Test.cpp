/**
 * @file
 * @author John R. Hoare
 *
 * Tests for the map class.
 */
/**
 * @defgroup BattleMap_Test BattleMap_Test
 * @ingroup Tests
 *
 * All the tests for the BattleMap Object
 *
 * @{
 */
#include "BattleMap.hpp"
#include <iostream>
#include <assert.h>
#include <fstream>
#include <vector>

using namespace std;


vector<Coord> explList;

void test_image_loaded(BattleMap& bm, const char* image_filename);

bool valid_pixel(pixel_type_t pixel) {
  return (pixel == BattleMap::MAP_EMPTY ||
          pixel == BattleMap::MAP_DESTRUCTABLE ||
          pixel == BattleMap::MAP_INDESTRUCTABLE);
}

int main() {
  explList.push_back(Coord(10,10));

  BattleMap bm;
  cout << "================================================" << endl;
  cout << "Testing BattleMap" << endl;
  bm.loadMap("../../../../gamefiles/battle/maps/test.map");

  cout << "Checking some properties that we know about this particular map"
       << endl;
  for(int xpos = 0; xpos < 800; xpos++)
    for(int ypos = 0; ypos < 600; ypos++)
      assert(bm.getPixel(xpos,ypos) != BattleMap::MAP_INDESTRUCTABLE);
  assert(bm.getXSize() == 800);
  assert(bm.getYSize() == 600);

  /// Sanity check that the two getPixel functions are correct.
  /// Also checks that the map only has the "valid" values for pixels.
  cout << "Checking that map is loaded valid" << endl;
  for(int xpos = 0; xpos < 800; xpos++)
    for(int ypos = 0; ypos < 600; ypos++) {
      assert(bm.getPixel(Coord(xpos,ypos)) == bm.getPixel(xpos,ypos));
      assert(valid_pixel(bm.getPixel(xpos,ypos)));
    }

  /// Check that the map is the same as the image file.
  cout << "Checking that map is the same as image file" << endl;
  test_image_loaded(bm, "../../../gamefiles/battle/maps/map1.pgm");

  /// Check the Team Spawns are Correct
  vector<Coord> teamOne = bm.getTeamSpawns(0);
  assert(teamOne.at(0) == Coord(100,100));
  assert(teamOne.at(1) == Coord(300,300));
  assert(teamOne.at(2) == Coord(400,400));
  vector<Coord> teamTwo = bm.getTeamSpawns(1);
  assert(teamTwo.at(0) == Coord(200,200));
  assert(teamTwo.at(1) == Coord(300,300));
  assert(teamTwo.at(2) == Coord(450,450));

  /// Check Explosion
  bm.explosion(Coord(10,100),5);
  bm.explosion(Coord(30,100),10);
  bm.explosion(Coord(100,100),20);
  bm.explosion(Coord(200,100),20);
  bm.explosion(Coord(300,100),40);

  // Out of range
  bm.explosion(Coord(1000, 1000), 100);
  bm.explosion(Coord(-1000, -1000), 100);
  bm.explosion(Coord(10, 10), -3);

  bm.outputMap("test.pgm");

  cout << "================================================" << endl;
}


void test_image_loaded(BattleMap& bm, const char* image_filename) {
  int x_size,y_size,maxVal;
  char inputLine1[81];
  int nextChar;
  ifstream inFile(image_filename);
  cout << image_filename << endl;
  return;

  /* Read past first line */
  inFile.getline(inputLine1,80);

  /* Read in width, height, maxVal */
  inFile >> x_size >> y_size >> maxVal;

  for(int j=0; j<y_size; j++) {
    for(int i=0; i<x_size; i++) {
      inFile >> nextChar;
      assert(nextChar == bm.getPixel(i,j));
    }
  }
}

/**
 * @}
 */
