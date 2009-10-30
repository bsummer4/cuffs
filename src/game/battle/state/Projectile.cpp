#include "Projectile.hpp"

Projectile::Projectile(int objID, int type, Coord coord, float xv, float yv) : GameObj(PROJECTILE, coord, xv, yv) , weapontype(type), id(objID) { radius = (float) type; }
