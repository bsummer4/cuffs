#include "state.hpp"
#include "sdl.hpp"
#include "misc.hpp"
#include "Interpreter.hpp"
#include "physics.hpp"

using namespace std;
using namespace game;
using namespace misc;

struct Hack {
  physics::Interpreter &i;
  physics::Simulation &s;
  Printer p;
  Hack (physics::Interpreter &i, physics::Simulation &s) : i(i), s(s) {}
  void handleEvent(string event) {
    i.handleEvent(event);
    LOOP {
      vector <string> stuff(s.move());
      if (!stuff.size()) break;
      FOREACH (vector <string>, it, stuff) {
        p.handleEvent(*it); }}}};

int main (int num_args, char **args) {
  chdir("../data");
  assert(num_args == 4 && "usage physics map x y");
  string map_filename = args[1];
  int x = atoi(args[2]), y = atoi(args[3]);
  sdl::SDL sdl(true);
  sdl.initVideo(1, 1, 32, "wtf");
  game::State s(args[1], "username", sdl);
  s.add(new game::Object("player-username", "player", x, y));
  physics::Simulation sim(s);
  physics::Interpreter simInt(sim, s);
  Printer p;
  Hack hack(simInt, sim);
  LineReader <typeof(hack)&> lr(cin, hack);
  lr.start();
  lr.join();

  return 0; }
