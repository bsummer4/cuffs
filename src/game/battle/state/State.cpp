#pragma once 
using namespace std;

State() {}

double State::changeWind(double newwind) {
  wind = newwind;
  return newwind;
} //RETURNS NEW SPEED, IF CHANGED

double State::getWind() {
  return wind;
} //RETURNS SPEED

void State::changePixel(int row, int column, MAPPIXEL newpixel) {
} //Changes the pixel at row, column to newpixel

MAPPIXEL State::getPixel(int row, int column) {
}  //Gets the pixel defined at row, column

bool State::setMap(string name) {
  map.open(mapname);
}  //Opens the map name

bool State::getMapName() {
  return mapname;
}  //returns map name

/*
   double wind;  //wind speed and direction--negative for left positive for right
   vector< vector< MAPPIXEL > > map;
   ifstream map;
   */
