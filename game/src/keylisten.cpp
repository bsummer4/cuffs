#include <unistd.h>
#include <iostream>
#include "keyListen.hpp"
#include "sdl.hpp"
#include "misc.hpp"
using namespace sdl;
using namespace std;

int main(int argc, char *argv[]) {
  assert(0 == chdir("../data"));
  SDL sdl(true);
  sdl.initVideo(800, 600, 32, "KeyListener");
  misc::Printer p;
  KeyListener <typeof(p)&> listen(p, sdl);
  sdl.registerEventHandler(listen);
  sdl.runEventLoop();
  return 0; }
