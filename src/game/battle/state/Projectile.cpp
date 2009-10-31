#include "Projectile.hpp"
using namespace std;
using namespace GameObject;

Projectile::Projectile(string objID, string type, Coord coord, float xv, float yv) : GameObj(objID, PROJECTILE, coord, xv, yv) , weapontype(type) { radius = 5;}
