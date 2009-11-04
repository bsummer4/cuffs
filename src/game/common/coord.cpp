#include "coord.hpp"

Coord::Coord(int xcoord, int ycoord) : x(xcoord), y(ycoord) { }
Coord::Coord() {
  x = 0;
  y = 0;
}

bool Coord::operator==(const Coord &other) const {
  return this->x == other.x && this->y == other.y;
}
