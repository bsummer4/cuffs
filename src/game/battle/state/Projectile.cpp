#include "Projectile.hpp"

Projectile::Projectile(int type, Coord coord) : GameObj(PROJECTILE, coord) , weapontype(type) { }
