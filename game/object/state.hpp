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
    GlobalObject (const string &map_filename, sdl::SDL &sdl)
      : map(MapLoader(map_filename, sdl)),
        wind(0), gravity(0),
        mapname(map_filename),
        Object("global", "global", 0, 0) {}
    string mapname;
    Map map;
    float wind, gravity; };

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
    output.push_back("draw global 0 0");
    FOREACH (State::id_objects, it, s.objects) {
      Object &object = *(it->second);
      string type = object.type;
      if (!type.compare("global")) continue; // This goes first
      ostringstream o;
      o << "draw " << type << " " << object.x << " " << object.y;
      output.push_back(o.str()); }
    //output.push_back("flip");
    // cerr << "drawing messages:" << endl;
    // FOREACH (vector <string>, it, output) cerr << "    " << *it << endl;
    return output; };};
