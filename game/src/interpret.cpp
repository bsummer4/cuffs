/// @addtogroup Interpret
/// @ingroup Game
/// @section Description
/// Reads messages from stdin an sends them to the Interpreter.  We
/// try to open a map 'example.map' in the working directory, so make
/// sure there's one there.

#include <assert.h>
#include <unistd.h>
#include <stdexcept>
#include "cmb.hpp"
#include "Connection.hpp"
#include "Interpreter.hpp"
#include "misc.hpp"
#include "lib.h"
using namespace std;

int main (int argc, char **argv) {
  assert(!chdir("../data"));
  sdl::SDL sdl(true);
  game::State state("example.map", "username", sdl);
  game::Interpreter i(state);
  misc::LineReader <game::Interpreter> r(cin, i);
  r.start();
  r.join();
  return 0; }
