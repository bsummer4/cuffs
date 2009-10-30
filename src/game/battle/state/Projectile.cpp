#include "Projectile.hpp"
using namespace std;
using namespace GameObject;

Projectile::Projectile(int objID, int type, Coord coord, float xv, float yv) : GameObj(objID, PROJECTILE, coord, xv, yv) , weapontype(type) { radius = (float) type; }
