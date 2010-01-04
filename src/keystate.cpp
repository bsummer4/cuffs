/// @addtogroup keystate
/// @ingroup Game
/// @section Description
/// Every time a key is pressed or released, print a line which is a
/// whitespace-delimited list of all the currently held keys.

#include <unistd.h>
#include <iostream>
#include "keyState.hpp"
#include "sdl.hpp"
#include "misc.hpp"
using namespace sdl;
using namespace std;

void print_keystate(KeyState &state) { state.pprint(cout); }

int main(int argc, char *argv[]) {
  assert(!chdir(DATADIRR));
  SDL sdl(true);
  sdl.initVideo(100, 100, 32, "KeyListener");
  string important[] = {"w", "a", "s", "d", "left", "right", "up", "down",
                        "leftmousebutton", "rightmousebutton"};
  KeyState state(sdl, print_keystate,
                 set <string> (important, important + 10));
  sdl.registerEventHandler(state);
  sdl.runEventLoop();
  return 0; }
