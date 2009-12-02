#include <unistd.h>
#include <iostream>
#include "sdl.h++"
#include "misc.h++"
using namespace misc;
using namespace std;

int main() {
  chdir("../data");
  sdl::SDL s(true, true);
  s.initVideo(800, 600, 32, "Rendering Utillity");
  s.initAudio();
  sdl::Renderer render(s);
  LineReader <typeof(render) &> read(cin, render);
  read.start();
  read.join();
  return 0; }
