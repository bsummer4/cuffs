/// Reads messages from stdin an sends them to the Interpreter.  We
/// try to open a map 'example.map' in the working directory, so make
/// sure there's one there.

#include <assert.h>
#include <unistd.h>
#include <stdexcept>
#include "cmb.h++"
#include "Connection.hpp"
#include "Interpreter.h++"
#include "misc.h++"
#include "lib.h"
using namespace std;

int main (int argc, char **argv) {
  chdir("../data");
  sdl::SDL sdl(true);
  try {
    game::State state("example.map", "username", sdl);
    game::Interpreter i(state);
    misc::LineReader <game::Interpreter> r(cin, i);
    r.start();
    r.join();
    return 0; }
  catch (runtime_error &e) {
    cerr << "error: " << e.what() << endl;
    return -1; }}
