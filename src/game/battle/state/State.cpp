#include "State.hpp"

State::State() {}

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

string State::setMap(string name) {
  mapname = name;
}//Opens the map name

string State::getMapName() {
  return mapname;
}//returns map name

void State::startBattle() {
  setupMap();
  battlestarted = 1;
}//Not implemented yet

void State::stopBattle() {
  battlestarted = 0;
}//Not implemented yet

bool State::getBattleState() {
  if(battlestarted == 0)
    return false;
  else
    return true;
}

bool State::changeWeapon(int weapon) {
  weaponid = weapon;
}//sets current weapon

int State::getWeapon() {
  return weaponid;
}//returns current weapon

void State::setupMap() {
  if(mapname.length() == 0)
  {
    cerr << "No map defined when trying to set up the map." << endl;
    return;
  }
}
