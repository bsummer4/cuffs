/// Open an SDL window and draw things on it as drawing commands come
/// in from stdin.

#include <unistd.h>
#include <iostream>
#include "sdl.hpp"
#include "misc.hpp"
using namespace misc;
using namespace std;

int main(int argc, char *argv[]) {
  assert(0 == chdir("../data"));
  sdl::SDL s(true, true);
  s.initVideo(800, 600, 32, "Rendering Utillity");
  s.initAudio();
  sdl::Renderer render(s);
  LineReader <typeof(render) &> read(cin, render);
  read.start();
  read.join();
  return 0; }
