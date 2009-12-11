#pragma once
#include <SDL/SDL.h>
#include <string>
#include <stdexcept>
#include <sstream>
#include <cstdlib>
#include "sdl.hpp"
#include "macro.h"

namespace sdl {
  using namespace std;

  /// Modifiers
  const int num_mod_types = 4;
  const int mod_types[] = {
    KMOD_CTRL, KMOD_SHIFT, KMOD_ALT, KMOD_META };
  const string mod_names[] = {
    "ctrl", "shift", "alt", "meta"};

  /// Modifier keys
  const int num_mod_keys = 11;
  const SDLKey mod_keys[] = {
    SDLK_SCROLLOCK, SDLK_RSHIFT, SDLK_LSHIFT, SDLK_RCTRL, SDLK_LCTRL,
    SDLK_RALT, SDLK_LALT, SDLK_RMETA, SDLK_LMETA, SDLK_RSUPER, SDLK_LSUPER };

  /// When given SDL Keypres events, sends the name of the key to the
  /// event handler, supports mouse movement and mouse button-presses.
  /// @TODO Describe the relationship between keys and their string
  /// names.  Probably just a reference to some SDL docs.
  template <typename H>
  class KeyListener {
      H handler;
      SDL &sdl;

  public:
    KeyListener(H handler, SDL &sdl) : handler(handler), sdl(sdl) {};
    ~KeyListener() {}

    void operator()(SDL_Event& event) {
      switch (event.type) {
      case SDL_KEYDOWN: {
        SDLKey key = event.key.keysym.sym;
        if (isModKey(key)) return;
        handler.handleEvent(getModifiers(SDL_GetModState()) +
                            SDL_GetKeyName(key)); }
        break;
      case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT)
          handler.handleEvent("leftmousebutton");
        break;
      case SDL_MOUSEMOTION: {
          ostringstream o;
          o << "mouse " << event.motion.x << " " << event.motion.y;
          handler.handleEvent(o.str()); }}}

  private:
    string getModifiers(SDLMod mod) {
      // No modifier keys pressed.
      if (mod == KMOD_NONE) return "";

      // Check for the presence of each SDLMod value
      string result;
      ITER (ii, 0, num_mod_types)
        if (mod & mod_types[ii])
          result += mod_names[ii] + " ";
      return result; }

    bool isModKey(SDLKey &key) {
      ITER (ii, 0, num_mod_keys)
        if (mod_keys[ii] == key)
          return true;
      return false; }};}
