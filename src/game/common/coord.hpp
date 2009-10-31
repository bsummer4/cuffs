#pragma once 

/** 
 * @defgroup Common Common
 * Everything that goes into the game backend. 
 */

/** 
 * @defgroup Coord Coord
 * @ingroup Common
 * Coord class, holds x & y coordinates in one class.
 */

/** 
 * @addtogroup Coord
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
