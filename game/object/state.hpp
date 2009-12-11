#pragma once
#include <stdexcept>
#include <map>
#include <iostream>
#include <string.h>
#include "Object.hpp"
#include "Map.hpp"
#include "sdl.hpp"
#include "macro.h"

namespace game {
  using namespace std;

  /// All the types we use in the state;
  const char *types[10] = {"map", "player", "rock", NULL};

  class GlobalObject : public Object {
  public:
    string mapname;
    Map map;
    float wind, gravity;
    GlobalObject (const string &map_filename, sdl::SDL &sdl)
      : Object("global", "global", 0, 0),
        mapname(map_filename),
        map(MapLoader(map_filename, sdl)),
        wind(0), gravity(0) {}

    virtual void setProperty(const string &property, const string &value) {
      if (property == "wind") wind = atoi(value.c_str());
      else if (property == "gravity") gravity = atoi(value.c_str());
      else this->Object::setProperty(property, value); }

    virtual string getProperty(const string &property) {
      ostringstream o;
      if (property == "wind") { o << wind; return o.str(); }
      else if (property == "gravity") { o << gravity; return o.str(); }
      return this->Object::getProperty(property); }};

  class State {
  public:
    typedef map <string, Object*> id_objects;
    GlobalObject *global;
    id_objects objects;
    const string username;

    State(const string &map_filename, const string username, sdl::SDL &sdl)
      : global(new GlobalObject(map_filename, sdl)), username(username) {
      objects["global"] = global; }
    bool player_alive() { return has("player-" + username); }
    Object &player() { return (*this)["player-" + username]; }
    bool has(const string &id) { return  objects.count(id); }
    Object &operator[] (const string &id) {
      if (!has(id)) throw runtime_error("No object with given id");
      return *objects.find(id)->second; }
    void remove(string id) { objects.erase(id); }
    void add(Object *o) {
      if (objects.count(o->id))
        throw runtime_error("Id conflict.  ");
      objects[o->id] = o; }};

  vector <string> render_state(game::State &s, vector <string> &output) {
    FOREACH (State::id_objects, it, s.objects) {
      Object &object = *(it->second);
      string type = object.type;
      if (!type.compare("global")) continue; // This goes last
      ostringstream o;
      o << "draw-center " << type << " " << object.x << " " << object.y;
      output.push_back(o.str()); }
    output.push_back("draw global 0 0");
    return output; };};
