#pragma once
#include <sstream>
#include <iostream>
#include <stdexcept>
#include "state.hpp"

namespace game {
  using namespace std;
  typedef enum { NEW, DELETE, SET, QUERY, EXPLODE, MOVE, MAP, INVALID,
                 MESSAGE } command_hash_t;
  command_hash_t hashCommand(string &command) {
    if (command.at(0) != '/') return MESSAGE;
    if (!command.compare(string("/new"))) return NEW;
    if (!command.compare(string("/delete"))) return DELETE;
    if (!command.compare(string("/set"))) return SET;
    if (!command.compare(string("/query"))) return QUERY;
    if (!command.compare(string("/explode"))) return EXPLODE;
    if (!command.compare(string("/move"))) return MOVE;
    if (!command.compare(string("/map"))) return MAP;
    return INVALID;}

  class Interpreter {
  public: 
  public:
    Interpreter(State &state) : state(state) {}
    void handleEvent(const string &event) {
      cerr << "-> " << event << endl;
      istringstream i(event);

      // Get the command from input
      string command;
      i >> command;
      if (!command.compare("")) return;

      // Dispatch on the command name
      switch (hashCommand(command)) {
      case NEW: { string id, type;
                  int x, y;
                  i >> id >> type >> x >> y;
                  state.add(new Object(id, type, x, y)); }
        break;
      case DELETE: { string object;
                     i >> object;
                     if (!object.compare("global"))
                       throw runtime_error("You can't do that!");
                     state.remove(object); }
          break;
      case SET: { string object, property, value;
                  i >> object >> property >> value;
                  state[object].setProperty(property, value); }
        break;
      case QUERY: { string object, property, value;
                    i >> object >> property;
                    cerr << state[object][property] << endl; }
        break;
      case MAP: { string name;
                  i >> name;
                  state["global"].setProperty("map", name); }
        break;
      case MOVE: { string object;
                   int x, y;
                   i >> object >> x >> y;
                   state[object].x = x;
                   state[object].y = y; }
        break;
      case EXPLODE: { int x, y, radius;
                      i >> x >> y >> radius;
                      state.global->map.explosion(x, y, radius); }
        break;
      case MESSAGE: {
        cerr << "Message: " << event << endl; }
        break;
      case INVALID:
      default:
        throw runtime_error("Invalid Command");}}
    State &state;};}
