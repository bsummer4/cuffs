/// Copies an arbitrary image and writes it to stdout as a pgm image

#include <string>
#include "Map.h++"
#include "sdl.h++"
using namespace std;

int main (int num_args, char **argv) {
  assert(num_args == 2 && "usage: copy-pgm mapfile");
  chdir("../data");
  sdl::SDL sdl(true);
  sdl.initVideo(0, 0, 32, "TODO: get rid of this line.  ");
  string filename = argv[1];
  filename += ".map";
  game::MapLoader ml(filename, sdl);
  game::Map m(ml);
  game::map_to_pgm(m, cout); }