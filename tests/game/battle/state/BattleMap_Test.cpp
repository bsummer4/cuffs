/**
 * @file 
 * @author John R. Hoare
 *
 * Tests for the map class. 
 */
/**
 * @defgroup BattleMap_Test
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

void test_image_loaded(BattleMap& bm);

int main(){
    explList.push_back(Coord(10,10));

    BattleMap bm;
    cout << "================================================" << endl;
    cout << "Testing BattleMap" << endl;
    // /home/john/dashinggents_fistacuffs/trunk/tests/game/battle/state
    bm.loadMap("../../../../gamefiles/battle/maps/test.map");

    /// Check correct width and height. 
    cout << "Checking correct width and height" << endl;
    assert(bm.getXSize() == 800);
    assert(bm.getYSize() == 600);

    /// Sanity check that the two getPixel functions are correct.
    /// Also checks that the map only has the "valid" values for pixels. 
    cout << "Checking that map is loaded valid" << endl;
    // Check that the two getPixel's Agree
    for (int xpos = 0; xpos < 800; xpos++){
        for (int ypos = 0; ypos < 600; ypos++){
            assert(bm.getPixel(Coord(xpos,ypos)) == bm.getPixel(xpos,ypos));
            assert(bm.getPixel(xpos,ypos) == MAP_EMPTY ||
                   bm.getPixel(xpos,ypos) == MAP_DESTRUCTABLE);
            // On this map, there's no indestructable
            assert(bm.getPixel(xpos,ypos) != MAP_INDESTRUCTABLE);
        }
    }

    /// Check that the map is the same as the image file.
    cout << "Checking that map is the same as image file" << endl;
    test_image_loaded(bm);

    /// Check the Team Spawns are Correct

    /// Check Explosion
    bm.explosion(Coord(10,10),5);
    bm.outputMap("test.pgm");

    // This map is 800x600 so test that.
    cout << "================================================" << endl;
}


void test_image_loaded(BattleMap& bm){
    int x_size,y_size,maxVal;
    char inputLine1[81];
    char nextChar;
    ifstream inFile("../../../gamefiles/battle/maps/map1.pgm");

    /* Read past first line */
    inFile.getline(inputLine1,80);

    /* Read in width, height, maxVal */
    inFile >> x_size >> y_size >> maxVal;

    for (int i=0; i<x_size; i++){
        for (int j=0; j<y_size; j++) {
            inFile >> nextChar;
            assert(nextChar == bm.getPixel(i,j));
        }
    }
}


/**
 * @}
 */
