#include "State.hpp"

State::State() {}

/*
double State::changeWind(double newwind) {
  wind = newwind;
  return newwind;
}//RETURNS NEW SPEED, IF CHANGED

double State::getWind() {
  return wind;
}//RETURNS SPEED

void State::changePixel(int row, int column, MAPPIXEL newpixel) {
}//Changes the pixel at row, column to newpixel

MAPPIXEL State::getPixel(int row, int column) {
}//Gets the pixel defined at row, column
*/

static const int DEBUG = 1;

string State::setMap(string name) {
  if(DEBUG)
    cerr << "setMap called with " << name << endl;
  mapname = name;
  return mapname;
}//Opens the map name

string State::getMapName() {
  if(DEBUG)
  cerr << "getMapName called:Mapname = " << mapname << endl;
  return mapname;
}//returns map name

void State::startBattle() {
  if(DEBUG)
    cerr << "startBattle called" << endl;
  setupMap();
  battlestarted = 1;
}//Not implemented yet

void State::stopBattle() {
  if(DEBUG)
    cerr << "stopBattle called" << endl;
  battlestarted = 0;
}//Not implemented yet

bool State::getBattleState() {
  if(battlestarted == 0)
    return false;
  else
    return true;
}

bool State::changeWeapon(int weapon) {
  if(DEBUG)
    cerr << "changeWeapon called with " << weapon << endl;
  weaponid = weapon;
}//sets current weapon

int State::getWeapon() {
  if(DEBUG)
    cerr << "getWeapon called" << endl;
  return weaponid;
}//returns current weapon

void State::setupMap() {
  if(DEBUG)
    cerr << "setupMap called" << endl;
  if(mapname.length() == 0)
  {
    cerr << "No map defined when trying to set up the map." << endl;
    return;
  }
}
