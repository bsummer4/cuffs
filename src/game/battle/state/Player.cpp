#include "Player.hpp"

Player::Player(int objtype, Coord coord, int t) : GameObj(objtype, coord), team(t) { }
Player::Player(int objtype, int x, int y, int t) : GameObj(objtype, x, y), team(t) { }
