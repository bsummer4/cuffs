#include "Player.hpp"
using namespace std;
using namespace GameObject;

Player::Player(int objID, string pname, Coord coord, int teamnum, int inithealth) : GameObj(objID, PLAYER, coord, 0, 0), team(teamnum), health(inithealth), name(pname) {}
void Player::hit(int damage) { health -= damage; };
