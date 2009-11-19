#pragma once

/**
 * @addtogroup Common
 * @{
 */


/**
 * Coord virtual class.
 *
 */
class Coord {
public:
  Coord();
  Coord(int xcoord, int ycoord);
  int x;
  int y;
  bool operator==(const Coord &other) const;
};

/**
 * @}
 */
