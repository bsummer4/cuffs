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

static int explosion_decrease = 4;

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
  vector<physics::Explosion> explosion_list;
  bool new_explosion;
  bool new_shot;
  UserInterface() : cursor(0, 0), new_explosion(false) {}
  void handleEvent(const string &event){
    istringstream i(event);

    // Get the command from input
    string command;
    i >> command;
    if (!command.compare("")) return;
    game::command_hash_t cmd = game::hashCommand(command);
    switch(cmd){
    case game::EXPLODE:
      int x,y,radius;
      i >> x >> y >> radius;
      explosion_list.push_back(physics::Explosion(x,y,radius));
      new_explosion = true;
    break;
    case game::NEW:
      // Ignore the event of the player starting
      if ( event.find("player-") == string::npos ) new_shot = true;
    break;
    default:
    break;}}

  void render(State &state, vector <string> &output) {
    if (!state.player_alive()) return;
    game::Object &player = state.player();
    { ostringstream line, circle;
      line << "line 2 0 255 0 "
           << cursor.x << " " << cursor.y << " "
           << player.x << " " << player.y;
      circle << "circle 4 0 255 0 " << cursor.x << " " << cursor.y;
      output.push_back(line.str());
      output.push_back(circle.str()); }
    // @TODO hack
    if ( new_explosion ){
      output.push_back("play explode");
      new_explosion=false; }
    if ( new_shot ){
      output.push_back("play shot");
      new_shot = false;}
    FORII(explosion_list.size()){
      physics::Explosion &e = explosion_list[ii];
      ostringstream explosion;
      explosion << "circle " << e.radius << " 255 0 0 " << e.x << " " << e.y;
      output.push_back(explosion.str());
      e.radius -= explosion_decrease;
      if ( e.radius <= 0 ) {
        explosion_list.erase(explosion_list.begin()+ii);
        ii--;}}}};

struct MouseHandler {
  sdl::SDL &sdl;
  UserInterface &ui;
  MouseHandler(sdl::SDL &sdl, UserInterface &ui) : sdl(sdl), ui(ui) {}
  void operator()(SDL_Event& e) {
    if (e.type != SDL_MOUSEMOTION) return;
    //cerr << "mouse motion.  ";
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
  int iteration;
  O output;
  cmb::TimestampAdder ta;
  int game_time;
  UserInterface &ui;

public:
  Pipeline(MsgQueue &gameInQ, MsgQueue &userInQ,
           sdl::Renderer &r, Interpreter &i, State &state,
           physics::Simulation &sim_, O output, UserInterface &ui)
    : gameInQ(gameInQ), userInQ(userInQ), r(r), i(i), state(state), sim(sim_),
      simInt(sim, state), iteration(0), output(output), ta(0), game_time(0),
      ui(ui) {}
  void operator()(SDL_Event& e) {
    //cerr << ".";
    int new_game_time = (SDL_GetTicks() - start_time) / game_interval;
    if (new_game_time <= game_time) return;
    int old_time = game_time;
    game_time = new_game_time;

    vector <string> stateChangeMsgs = gameInQ.popAll();
    handleAll <typeof(i)&> (stateChangeMsgs, i);
    handleAll <typeof(ui)&>(stateChangeMsgs, ui);
    vector <string> userEvents = userInQ.popAll();
    handleAll <typeof(simInt)&> (userEvents, simInt);
    handleAll <typeof(simInt)&> (stateChangeMsgs, simInt);
    ITER (time, old_time + 1, new_game_time + 1) {
      vector <string> stuff = sim.move();
      ta.time = time;
      vector <string> output_messages = ta(stuff);
      handleAll <typeof(output)&> (output_messages, output); }
    vector <string> render_messages;
    render_state(state, render_messages);
    ui.render(state, render_messages);
    handleAll <typeof(r)&> (render_messages, r);
    r.flip();
    iteration++; }};

void ref_handshake(string username, PlayerMap &players, string &map,
                   int &ref) {
  cout << "/identify " << username << endl;
  LOOP {
    char buffer[1024];
    if (!cin.getline(buffer, 1024)) assert(false && "badness");
    istringstream i(buffer); int from; string command;
    i >> from >> command;
    if (!command.compare("/identify")) continue;
    if (!command.compare("/map")) { i >> map; continue; }
    if (!command.compare("/start")) {
      int id; char colon; string player;
      while (i >> id >> colon >> player)
        players[id] = player;
      if (!i.eof()) throw runtime_error("syntax error in /start message");
      ref = from;
      return; } 
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
  assert(0 == chdir("../data"));
  sdl::SDL sdl(true, true);
  sdl.initVideo(800, 600, 32, "Rock-Throwing Game");
  sdl.initAudio();

  // Ref Handshake
  PlayerMap players;
  string mapname;
  int ref_address;
  ref_handshake(username, players, mapname, ref_address);
  start_time = SDL_GetTicks();

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
  render.new_image("player", "player.pgm");
  render.new_image("projectile", "rock.pgm");
  render.new_sound("explode", "explosion3.wav");
  render.new_sound("shot", "shot2.wav");
  render.draw("global", 0, 0);
  render.flip();
  game::Interpreter i(state);

  // Gather up the process numbers of the players
  vector <int> clients;
  clients.push_back(ref_address);
  FOREACH (PlayerMap, it, players)
    clients.push_back((*it).first);

  // IO and Pipeline
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
  SDL_TimerID timer;
  timer = SDL_AddTimer(40, gameLoopTimer, NULL);

  // Run everything
  lr.start();
  sdl.runEventLoop();

  return 0; }
