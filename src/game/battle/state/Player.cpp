#include "Player.hpp"

Player::Player(int objtype, Coord coord, int teamnum) : GameObj(objtype, coord), team(teamnum) { health = 10;}
Player::Player(int objtype, int x, int y, int teamnum) : GameObj(objtype, x, y), team(teamnum) { health = 10;}
void Player::hit(int damage) { health -= damage; };
