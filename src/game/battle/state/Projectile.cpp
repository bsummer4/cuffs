#include "Projectile.hpp"
using namespace std;
using namespace GameObject;

Projectile::Projectile(std::string objID, std::string type, Coord coord, float xv, float yv) : GameObj(objID, PROJECTILE, coord, xv, yv) , weapontype(type) {
  radius = 5;
}
