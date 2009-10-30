#include "State.hpp"
using namespace std;
using namespace GameObject;

State::State() { gravity = 9.8;}

double State::changeWind(float newwind) {
  wind = newwind;
  return newwind;
}//RETURNS NEW SPEED, IF CHANGED

double State::getWind() {
  return wind;
}//RETURNS SPEED

///Creates an explosion based on which object_id it hit with
void State::projHit(Coord coord, Projectile &proj) {
  if(DEBUG)
    cerr << "hitObj called with a coord" << endl;
  bmap.explosion(coord, proj.radius);
}

///Method for getting the pixel at Coordinate coord
pixel_type_t State::getPixel(Coord coord) {
  if(DEBUG)
    cerr << "getPixel called with a coord" << endl;
  if(mapname.length() == 0)
  {
    cerr << "No map defined when trying to get a pixel from the map." << endl;
    return 0;
  }
  bmap.getPixel(coord);
}//Gets the pixel defined at coord

///Method for getting the pixel at Coordinate coord
pixel_type_t State::getPixel(int x, int y) {
  if(DEBUG)
    cerr << "getPixel called with x" << x << "and y" << y << endl;
  if(mapname.length() == 0)
  {
    cerr << "No map defined when trying to get a pixel from the map." << endl;
    return 0;
  }
  bmap.getPixel(Coord(x,y));
}//Gets the pixel defined at coord

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
  bmap.loadMap(mapname);
}

void State::addPlayer(int objID, string name, Coord coord, int team, int health) {
  if(DEBUG)
    cerr << "addPlayer called" << endl;
  Player *play = new Player(objID, name, coord, team, health);
  players.insert(make_pair(objID, play));
}

void State::addProjectile(int projID, int weapontype, int x, int y, float xvel, float yvel) {
  Projectile *proj = new Projectile(projID, weapontype, Coord(x, y), xvel, yvel);
  GameObj *go = dynamic_cast<GameObj *>(proj);
  projectiles.insert(make_pair(projID, proj));
  myprojectiles.insert(make_pair(projID, proj));
  objects.insert(make_pair(projID, go));
}

void State::addProjectile(int projID, int weapontype, Coord coord, float xvel, float yvel) {
  Projectile *proj = new Projectile(projID, weapontype, coord, xvel, yvel);
  GameObj *go = dynamic_cast<GameObj *>(proj);
  projectiles.insert(make_pair(projID, proj));
  myprojectiles.insert(make_pair(projID, proj));
  objects.insert(make_pair(projID, go));
}

float State::getGravity() {
  return gravity;
}

void State::setGravity(float newgrav) {
  gravity = newgrav;
}

void State::moveObj(int obj_id, int x, int y) {
  map<int, GameObject::GameObj *>::iterator it;
  it = objects.find(obj_id);
}

void State::hitObj(int obj_id, int x, int y) {
  map<int, GameObject::GameObj>::iterator it;
}
