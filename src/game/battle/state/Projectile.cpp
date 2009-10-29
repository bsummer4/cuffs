#include "Projectile.hpp"

Projectile::Projectile(int objtype, Coord coord, float rad) : type(objtype), location(Coord), radius(rad){ }
Projectile::Projectile(int objtype, int x, int y, float rad) : type(objtype), location.x(x), location.y(y), radius(rad) { }
