#include "GameObject.hpp"
#include "Projectile.hpp"
#include "Player.hpp"
#include "coord.hpp"
#include "assert.h"
using namespace std;
using namespace GameObject;

int main(int argc, char **argv) {

  string projname("lofty-laser-1");
  string projtype("lazer");
  Projectile proj(projname, projtype, Coord(0,0), 5, 5);
  Coord coord;

  //-1*1+5*1 + 0
  //-1*4+5*2 + 0
  //-1*9+5*3 + 0
  //-1*16+5*4 + 0
  //-1*25+5*5 + 0
  coord = proj.movePhysics(0, -2, 0);
  fprintf(stderr, "X: %d Y: %d --- Should be X:0 Y:0\n", coord.x, coord.y);
  assert(coord == Coord(0,0));
  coord = proj.movePhysics(1, -2, 0);
  fprintf(stderr, "X: %d Y: %d --- Should be X:5 Y:4\n", coord.x, coord.y);
  assert(coord == Coord(5,4));
  coord = proj.movePhysics(2, -2, 0);
  fprintf(stderr, "X: %d Y: %d --- Should be X:10 Y:6\n", coord.x, coord.y);
  assert(coord == Coord(10,6));
  coord = proj.movePhysics(3, -2, 0);
  fprintf(stderr, "X: %d Y: %d --- Should be X:15 Y:6\n", coord.x, coord.y);
  assert(coord == Coord(15,6));
  coord = proj.movePhysics(4, -2, 0);
  fprintf(stderr, "X: %d Y: %d --- Should be X:20 Y:4\n", coord.x, coord.y);
  assert(coord == Coord(20,4));
  coord = proj.movePhysics(5, -2, 0);
  fprintf(stderr, "X: %d Y: %d --- Should be X:25 Y:0\n", coord.x, coord.y);
  assert(coord == Coord(25,0));
}
