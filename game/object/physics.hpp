#pragma once
#include <algorithm>
#include "state.hpp"

/// Physics for projectils
namespace physics {
  using namespace std;

  struct Point {
    int x, y;
    Point() : x(0), y(0) {};
    Point(int x, int y) : x(x), y(y) {};
    bool operator!=(const Point other) {
      return x != other.x || y != other.y; }};

  bool on_map(game::Map &map, Point p) {
    Point min(0, 0), max(map.width - 1, map.height - 1);
    return !(p.x > max.x || p.x < min.x || p.y > max.y || p.y < min.y); }

  /// Find the point closest to p0 between p0 and p1 that is solid,
  /// and set result to it.  Return false if there was not such point.
  bool find_hit(game::Map &map, Point p0, Point p1, Point &result) {
    int x0 = p0.x, x1 = p1.x, y0 = p0.y, y1 = p1.y;
    vector <Point> hits;
    bool steep = abs(y1 - y0) > abs(x1 - x0);
    { int temp;
      if (steep) {
        temp = x0;
        x0 = y0;
        y0 = temp;
        temp = x1;
        x1 = y1;
        y1 = temp; }
      if (x0 > x1) {
        temp = x0;
        x0 = x1;
        x1 = temp;
        temp = y0;
        y0 = y1;
        y1 = temp; }}
    int deltax = x1 - x0;
    int deltay = abs(y1 - y0);
    float error = 0;
    float deltaerr = ((float)deltay)/((float)deltax);
    int ystep;
    int y = y0;
    ystep = (y0 < y1) ? 1 : -1;
    for (int x = x0; x < x1; x++) {
      Point current = steep ? Point(y, x) : Point(x, y);
      if (on_map(map,current) && map.is_solid(current.x, current.y))
        hits.push_back(current);
      error += deltaerr;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1.0; }}

    // Calculate the closest hit and return it.  Return false if there
    // was no hit
    if (!hits.size()) return false;
    float lowest_distance = INFINITY;
    FOREACH (vector <Point>, hit, hits) {
      float distance = hypotf(p0.x - hit->x, p0.y - hit->y);
      if (distance < lowest_distance) {
        result = *hit;
        lowest_distance = distance; }}
    return true; }

  struct Projectile {
    float x, y, dx, dy;
    Projectile(float x, float y, float dx, float dy)
      : x(x), y(y), dx(dx), dy(dy) {}
    Projectile() {} // Uninitialized!!
    void move() { x += dx; y += dy; }
    void accelerate(float ddx, float ddy) { dx += ddx; dy += ddy; }};

  namespace helper {
    string msg_new(string id, int x, int y) {
      ostringstream o;
      o << "/new " << id << " projectile " << x << " " << y;
      return o.str(); }
    string msg_move(string id, int x, int y) {
      ostringstream o; o << "/move " << id << " " << x << " " << y;
      return o.str(); }
    string msg_explode(int x, int y, int radius) {
      ostringstream o; o << "/explode " << x << " " << y << " " << radius;
      return o.str(); }
    string msg_delete(string id) {
      ostringstream o; o << "/delete " << id;
      return o.str(); }}

  class Simulation;

  // Virtual
  struct SmartProjectile {
  public:
    bool is_new;
    Simulation *sim;
    string id;
    Projectile p;
    SmartProjectile (Simulation *sim, string id,
                     float x, float y, float dx, float dy)
      : sim(sim), is_new(true), id(id), p(x, y, dx, dy) {}
    float x() { return p.x; };
    float y() { return p.y; };
    float dx() { return p.dx; };
    float dy() { return p.dy; };
    virtual void update(game::State&, vector <string>&, bool&) = 0; };

  /// Maintain projectils we (a client) are responsible for.  We
  /// examine things in the state object to determine their effects on
  /// our projectiles.  All the projectiles we maintain also have
  /// entries in the state object but with less information.
  ///
  /// we never change the state directly, instead we yeild messages
  /// describing the changes which can be given to the state
  /// interpreter (or broadcast over a network).
  class Simulation {
    typedef string id;
    typedef std::map <id, SmartProjectile*> Projectiles;
    Projectiles projectiles;
    game::Map &map; /// state.global.map.map
    game::State &state; /// Read-only
  public:
    bool playerMoved; /// Hack
    Simulation(game::State &s)
      : state(s), map(s.global->map), playerMoved(false) {}
    ~Simulation() { FOREACH (Projectiles, it, projectiles)
                      delete it->second; }
    void add(SmartProjectile *p) { projectiles[p->id] = p; }
    bool irrelevant(SmartProjectile *p) {
      Point min(-2 * map.width, -2 * map.height);
      Point max(3 * map.width, 3 * map.height);
      return (p->x() > max.x || p->x() < min.x ||
              p->y() > max.y || p->y() < min.y); }

    /// Update all projectiles and return a sequence of message for
    /// the interpreter to update the state.
    vector <string> move() {
      vector <string> erase_after_loop;
      vector <string> messages;
      if (playerMoved) {
        game::Object &player = state.player();
        messages.push_back(helper::msg_move(player.id, player.x, player.y));
        playerMoved = false; }
      FOREACH (Projectiles, it, projectiles) {
        id id(it->first);
        SmartProjectile *p = it->second;
        bool erase;
        p->update(state, messages, erase);
        if (erase) erase_after_loop.push_back(id); }
      FOREACH (vector <string>, it, erase_after_loop)
        projectiles.erase(*it);
      return messages; }};

  class Rock : public SmartProjectile {
    Rock (Simulation *sim, string id,
          float x, float y, float dx, float dy)
      : SmartProjectile(sim, id, x, y, dx, dy) {};

    /// Add whatever messages are needed to communicate our state
    /// changes.  if (erase) then we should be removed from the
    /// simulation.
    virtual void update(game::State &state, vector <string> &messages,
                        bool &erase) {
      erase = false;
      Point start(x(), y());

      if (is_new) {
        is_new = false;
        messages.push_back(helper::msg_new(id, p.x, p.y));
        return; }
      if (sim->irrelevant(this)) {
        erase = true;
        messages.push_back(helper::msg_delete(id));
        return; }

      // Modify ourself
      p.accelerate(state.global->wind, state.global->gravity);
      p.move();

      Point end(x(), y());
      { Point hit; // We are sure that this will be set in find_hit in
                   // all cases where we use it.
        if (find_hit(state.global->map, start, end, hit)) {
          erase = true;
          messages.push_back(helper::msg_delete(id));
          messages.push_back(helper::msg_explode(hit.x, hit.y, 50));
          return; }}
      messages.push_back(helper::msg_move(id, end.x, end.y)); }};

  SmartProjectile *make_projectile(Simulation *sim,
                                   const string type, const string id,
                                   float x, float y, float dx, float dy) {
    return NULL; }

  struct Interpreter {
    int count;
    game::State &state;
    Simulation &sim;
    Interpreter(Simulation &sim, game::State &state)
      : sim(sim), count(0), state(state) {};
    void handleEvent(string s) {
      //cerr << "&sim" << (unsigned long long) &sim << endl;
      //cerr << "&state" << (unsigned long long) &state << endl;
      //cerr << "Interpreter::Invoked!" << endl;
      //cerr << "got player????? " << "player-" << state.username << endl;
      //FOREACH (game::State::id_objects, it, state.objects)
      //cerr << "  " << &it->first << endl;
      if (!state.has("player-" + state.username)) return;
      game::Object &player = state["player-" + state.username];
      //cerr << "Interpreter::got us a player!" << endl;

      istringstream i(s);
      string command;
      if (!(i >> command)) return;

      if (!command.compare("shoot")) {
        string type;
        float dx, dy;
        if (!(i >> type >> dx >> dy)) return;

        ostringstream s_id;
        s_id << state.username << "-" << type << "-" << count++;
        string id = s_id.str();
        sim.add(make_projectile(&sim, type, id,
                                player.x, player.y, dx, dy)); }

      if (!command.compare("move")) {
        float dx, dy;
        i >> dx >> dy;
        /// @TODO hack!!!
        player.x += dx;
        player.y += dy;
        sim.playerMoved = true; }}};}
