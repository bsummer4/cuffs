#pragma once 
using namespace std;

State() {}

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

bool State::setMap(string name) {
  map.open(mapname);
}//Opens the map name

bool State::getMapName() {
  return mapname;
}//returns map name

void State::startBattle() {
  battlestarted = 1;
  return true;
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

bool State::changeWeapon(int weapon); {
  weaponid = weapon;
}//sets current weapon

int State::getWeapon() {
  return weaponid;
}//returns current weapon

/*
   double wind;  //wind speed and direction--negative for left positive for right
   vector< vector< MAPPIXEL > > map;
   ifstream map;
   int weaponid;
   */
