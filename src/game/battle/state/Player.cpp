#include "Player.hpp"

Player::Player(Coord coord, int teamnum, int inithealth) : GameObj(PLAYER, coord, 0, 0), team(teamnum), health(inithealth) {}
void Player::hit(int damage) { health -= damage; };
