/// Copies an arbitrary image and writes it to stdout as a pgm image.
/// Uses SDL for this.  Mostly for testing game::Map, and sdl::SDL.

#include <string>
#include "Map.hpp"
#include "sdl.hpp"
using namespace std;

int main (int num_args, char **argv) {
  assert(num_args == 2 && "usage: copy-pgm mapfile");
  assert(0 == chdir("../data"));
  sdl::SDL sdl(true);
  sdl.initVideo(1, 1, 32, "TODO: get rid of this line.  ");
  string filename = argv[1];
  filename += ".map";
  game::MapLoader ml(filename, sdl);
  game::Map m(ml);
  game::map_to_pgm(m, cout); }
