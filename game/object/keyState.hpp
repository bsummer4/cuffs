#pragma once
#include <SDL/SDL.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <stdexcept>
#include <sstream>
#include <cstdlib>
#include "sdl.hpp"
#include "macro.h"

namespace sdl {
  using namespace std;

  /// Maintain a public map from keynames to if they are currently
  /// pressed down.  We only keep track of keys that we are
  /// specifically told are important.  We don't treat mod keys
  /// differently from normal keys
  class KeyState {
    typedef boost::function <void (KeyState&)> Callback;
    Callback callback;
    SDL &sdl;

  public:
    map <string, bool> keys_down; // Read only plz

    ~KeyState() {}
    KeyState(SDL &sdl, Callback callback, set <string> used_keys)
      : callback(callback), sdl(sdl) {
      FOREACH (set <string>, key, used_keys)
        keys_down[*key] = false; }

    bool mouse_event(SDL_Event& event) {
      return (event.type == SDL_MOUSEBUTTONDOWN ||
              event.type == SDL_MOUSEBUTTONUP); }

    bool keyboard_event(SDL_Event& event) {
      return (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP); }

    bool down_event(SDL_Event& event) {
      return (event.type == SDL_KEYDOWN ||
              event.type == SDL_MOUSEBUTTONDOWN); }

    string keyname(SDL_Event& event) {
      if (mouse_event(event)) {
        if (event.button.button == SDL_BUTTON_LEFT)
          return "leftmousebutton";
        if (event.button.button == SDL_BUTTON_RIGHT)
          return "rightmousebutton";
        return ""; }
      else if (keyboard_event(event))
        return SDL_GetKeyName(event.key.keysym.sym);
      else
        throw runtime_error("Invalid key event"); }

    ostream &pprint(ostream& o) {
      typedef map <string, bool> DownKeys;
      FOREACH (DownKeys, key, keys_down)
        if (key->second)
          o << key->first << " ";
      o << endl;
      return o; }

    void operator() (SDL_Event& event) {
      if (!mouse_event(event) && !keyboard_event(event)) return;
      map <string, bool>::iterator keydown = keys_down.find(keyname(event));
      if (keydown == keys_down.end()) return;
      keydown->second = down_event(event);
      callback(*this); }};}
