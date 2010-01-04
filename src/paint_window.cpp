/// This program sends the names of all keys pressed to stdout and
/// passes everything it receives to the renderer.  The windows size
/// is hard-coded to 800x600.

#include <unistd.h>
#include <iostream>
#include "sdl.hpp"
#include "keyListen.hpp"
#include "misc.hpp"
using namespace std;
using namespace sdl;
using namespace misc;

// Flip the screen after every draw
struct Painter {
  Renderer &render;
  Painter(Renderer &render) : render(render) {};
  void handleEvent(string render_message) {
    render.handleEvent(render_message);
    render.flip(); }};

int main(int argc, char *argv[]) {
  assert(0 == chdir(DATADIRR));
  SDL sdl(true, true);
  sdl.initVideo(800, 600, 32, "Rendering Utillity");
  sdl.initAudio();

  Printer p;
  KeyListener <typeof(p)&> listen(p, sdl);
  Renderer render(sdl);
  Painter paint(render);
  LineReader <typeof(paint)&> read(cin, paint);
  read.start();
  sdl.registerEventHandler(listen);
  sdl.runEventLoop();
  read.join();
  return 0; }
