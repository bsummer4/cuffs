#include "Projectile.hpp"

Projectile::Projectile(int objtype, Coord coord, float rad) : GameObj(objtype, coord), radius(rad) { }
Projectile::Projectile(int objtype, int x, int y, float rad) : GameObj(objtype, x, y), radius(rad) { }
