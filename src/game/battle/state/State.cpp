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
*/

///Creates an explosion based on which object_id it hit with
/*
void State::hitObj(Coord coord, Projectile &proj) {
  if(DEBUG)
    cerr << "hitObj called with a coord" << endl;
  map.explosion(coord, proj.radius);
}
*/

///Method for getting the pixel at Coordinate coord
pixel_type_t State::getPixel(Coord coord) {
  if(DEBUG)
    cerr << "getPixel called with a coord" << endl;
  if(mapname.length() == 0)
  {
    cerr << "No map defined when trying to get a pixel from the map." << endl;
    return 0;
  }
  map.getPixel(coord);
}//Gets the pixel defined at coord

///Method for getting the pixel at row, column 
pixel_type_t State::getPixel(int row, int column) {
  if(DEBUG)
    cerr << "getPixel called with " << row << "row and " << column << "column" << endl;
  if(mapname.length() == 0)
  {
    cerr << "No map defined when trying to set up the map." << endl;
    return 0;
  }
  map.getPixel(row, column);
}//Gets the pixel defined at row, column

string State::setMap(string name) {
  if(DEBUG)
    cerr << "setMap called with " << name << endl;
  mapname = name;
  return mapname;
}//Sets the map filename to load

string State::getMapName() {
  if(DEBUG)
    cerr << "getMapName called:Mapname = " << mapname << endl;
  return mapname;
}

void State::startBattle() {
  if(DEBUG)
    cerr << "startBattle called" << endl;
  setupMap();
  battlestarted = true;
}

void State::stopBattle() {
  if(DEBUG)
    cerr << "stopBattle called" << endl;
  battlestarted = false;
}

bool State::getBattleState() {
  return battlestarted;
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
  map.loadMap(mapname);
}

void State::addObject(int objtype, int x, int y) {
}

void State::addObject(int objtype, Coord coord) {
}
