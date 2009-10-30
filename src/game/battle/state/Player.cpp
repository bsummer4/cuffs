#include "Player.hpp"

Player::Player(int objtype, Coord coord, int t) : GameObj(objtype, coord), team(t) { health = 10;}
Player::Player(int objtype, int x, int y, int t) : GameObj(objtype, x, y), team(t) { health = 10;}
void Player::hit(int damage) { health -= damage };
