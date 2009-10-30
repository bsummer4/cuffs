#include "Player.hpp"

Player::Player(string pname, Coord coord, int teamnum, int inithealth) : GameObj(PLAYER, coord, 0, 0), team(teamnum), health(inithealth), name(pname) {}
void Player::hit(int damage) { health -= damage; };
