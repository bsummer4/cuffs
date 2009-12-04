/// This takes game messages from stdin, interprets them and sends
/// render messages to stdout.

#include <sstream>
#include "state.hpp"
#include "sdl.hpp"
#include "misc.hpp"
#include "cmb.hpp"
#include "Interpreter.hpp"
#include "keyListen.hpp"
#include "physics.hpp"
#include "macro.h"
using namespace misc;
using namespace game;

typedef Queue <string> MsgQueue;
typedef map <int,string> PlayerMap;

static int start_time; // set in main
const static int game_interval = 50;

template <typename H>
struct InputHandler {
  H handler;
  InputHandler(H handler) : handler(handler) {}
  void handleEvent(string event) {
    string keys[5] = {"space", "left", "right", "up", "down"};
    string results[5] = {"shoot rock -10 -5",
                         "move -5 0", "move 5 0",
                         "move 0 -5", "move 0 5"};
    FORII(5)
      if (event == keys[ii])
        return handler.handleEvent(results[ii]); }};

struct UserInterface {
  Point cursor;
  UserInterface() : cursor(0, 0) {}
  void render(State &state, vector <string> &output) {
    cerr << "ui" << endl;
    if (!state.player_alive()) return;
    cerr << "UI" << endl;
    game::Object &player = state.player();
    { ostringstream line, circle;
      line << "line 2 0 255 0 "
           << cursor.x << " " << cursor.y << " "
           << player.x << " " << player.y;
      circle << "circle 4 0 255 0 " << cursor.x << " " << cursor.y;
      output.push_back(line.str());
      output.push_back(circle.str()); }
    output.push_back("flip"); }};

struct MouseHandler {
  sdl::SDL &sdl;
  UserInterface &ui;
  MouseHandler(sdl::SDL &sdl, UserInterface &ui) : sdl(sdl), ui(ui) {}
  void operator()(SDL_Event& e) {
    if (e.type != SDL_MOUSEMOTION) return;
    cerr << "mouse motion.  ";
    // Get the mouse offsets
    int x = e.motion.x, y = e.motion.y;
    ui.cursor = Point(x, y); }};

template <typename O>
class Pipeline {
  MsgQueue &gameInQ, &userInQ;
  sdl::Renderer &r;
  Interpreter &i;
  game::State &state;
  physics::Simulation &sim;
  physics::Interpreter simInt;
  cmb::TimestampAdder ta;
  O output;
  int iteration;
  int game_time;
  UserInterface &ui;

public:
  Pipeline(MsgQueue &gameInQ, MsgQueue &userInQ,
           sdl::Renderer &r, Interpreter &i, State &state,
           physics::Simulation &sim_, O output, UserInterface &ui)
    : gameInQ(gameInQ), userInQ(userInQ), r(r), i(i), state(state), sim(sim_),
      simInt(sim, state), iteration(0), output(output), ta(0), game_time(0), ui(ui) {}
  void operator()(SDL_Event& e) {
    cerr << ".";
    int new_game_time = (SDL_GetTicks() - start_time) / game_interval;
    if (new_game_time <= game_time) return;
    // TODO If we have moved forward multiple steps, then we need to
    // physics stuff every step in between.  Otherwise objects from
    // someone with a slow computer could move slower than from fast
    // computers
    game_time = new_game_time;
    cerr << "gametime++ -> " << game_time << endl;
    handleAll <typeof(i)&> (gameInQ, i);
    cerr << "1";
    vector <string> ignore = userInQ.popAll();
    handleAll <typeof(simInt)&> (ignore, simInt);
    FOREACH (vector <string>, it, ignore)
      cerr << "\t--" << *it << endl;
    cerr << "2";
    vector <string> stuff(sim.move());
    cerr << "3";
    ta.time = game_time;
    cerr << "4";

    vector <string> output_messages = ta(stuff);
    handleAll <typeof(output)&> (output_messages, output);
    cerr << "5";
    vector <string> render_messages;
    render_state(state, render_messages);
    ui.render(state, render_messages);
    handleAll <typeof(r)&> (render_messages, r);
    cerr << "6";
    iteration++; }};

void ref_handshake(string username, PlayerMap &players, string &map) {
  cout << "/identify " << username << endl;
  LOOP {
    char buffer[1024];
    if (!cin.getline(buffer, 1024)) assert(false && "badness");
    istringstream i(buffer); int from; string command;
    i >> from >> command;
    if (!command.compare("/identify")) {
      string name; i >> name;
      players[from] = name;
      continue; }
    if (!command.compare("/map")) { i >> map; continue; }
    if (!command.compare("/start")) return;
    throw runtime_error("Bad handshake message"); }}

Uint32 gameLoopTimer(Uint32 interval, void* param) {
  SDL_UserEvent event = {SDL_USEREVENT, 0, NULL, NULL};
  SDL_PushEvent((SDL_Event*)&event);
  return interval; }

int main(int num_args, char **args) {
  // Args
  assert(num_args == 2 && "usage: game user");
  string username(args[1]);

  // SDL
  chdir("../data");
  sdl::SDL sdl(true);
  sdl.initVideo(800, 600, 32, "Rock-Throwing Game");

  // Ref Handshake
  PlayerMap players;
  string mapname;
  ref_handshake(username, players, mapname);
  start_time = SDL_GetTicks();
  cerr << "Handshake done" << endl;

  if (!mapname.size()) throw runtime_error("No map given");
  mapname += ".map";

  // Game State and logic objects
  State state(mapname, username, sdl);
  state.global->wind = 0.5;
  state.global->gravity = 0.4;
  physics::Simulation sim(state);
  MsgQueue gameQ;
  sdl::Renderer render(sdl);
  render.inject_image("global", state.global->map.map);
  render.new_image("player", "team-red.ppm");
  render.new_image("projectile", "rock.pgm");
  render.draw("global", 0, 0);
  render.flip();
  game::Interpreter i(state);


  // Gather up the process numbers of the players
  vector <int> clients;
  FOREACH (PlayerMap, it, players)
    clients.push_back((*it).first);

  // IO and Pipeline
  cerr << "Starting Pipeline" << endl;
  MsgQueue inQ, userQ;
  Printer p;
  UserInterface ui;
  MouseHandler mh(sdl, ui);
  Pipeline <Printer&> pipeline(inQ, userQ, render, i, state, sim, p, ui);
  cmb::Synchronizer <typeof(inQ)&> sync(inQ, clients);
  LineReader <typeof(sync)&> lr(cin, sync);

  // Event handling
  InputHandler <typeof(userQ)&> h(userQ);
  sdl::KeyListener <typeof(h)&> listen(h, sdl);
  sdl.registerEventHandler(listen);
  sdl.registerEventHandler(pipeline);
  sdl.registerEventHandler(mh);
  SDL_TimerID timer = SDL_AddTimer(40, gameLoopTimer, NULL);

  // Run everything
  lr.start();
  cerr << "Running event loop" << endl;
  sdl.runEventLoop();

  return 0; }
