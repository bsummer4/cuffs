#include "Projectile.hpp"

Projectile::Projectile(int type, Coord coord, float xv, float yv) : GameObj(PROJECTILE, coord, xv, yv) , weapontype(type) { radius = (float) type; }
