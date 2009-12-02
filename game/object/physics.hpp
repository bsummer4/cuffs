#pragma once
#include <algorithm>
#include "state.hpp"

/// Physics for projectils
namespace physics {
  using namespace std;

  struct Point {
    int x, y;
    Point(int x, int y) : x(x), y(y) {};
    bool operator!=(const Point other) {
      return x != other.x || y != other.y; }};

  bool on_map(game::Map &map, Point p) {
    Point min(0, 0), max(map.width - 1, map.height - 1);
    return !(p.x > max.x || p.x < min.x || p.y > max.y || p.y < min.y); }

  /// Find the point closest to p0 between p0 and p1 that is solid,
  /// and set result to it.  return false if there was not such point.
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

    // Calculate the closest hit and return it.  Return p2 if there
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
    Projectile() {}
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
    typedef std::map <id, Projectile> Projectiles;
    Projectiles projectiles;
    vector <id> new_projectiles; /// Have not been '/new'ed yet
    game::Map &map; /// state.global.map.map
    game::State &state; /// Read-only
  public:
    bool playerMoved; /// Hack
    Simulation(game::State &s)
      : state(s), map(s.global->map), playerMoved(false) {}

    void set (id id, float x, float y, float dx, float dy) {
      //cerr << "*.set:" << id << endl;
      if (!projectiles.count(id))
        new_projectiles.push_back(id);
      Projectile p(x, y, dx, dy);
      projectiles[id] = p; }

    bool irrelevant(Projectile &p) {
      Point min(-2 * map.width, -2 * map.height);
      Point max(3 * map.width, 3 * map.height);
      return (p.x > max.x || p.x < min.x || p.y > max.y || p.y < min.y); }

    void notnew(const string id) {
      new_projectiles.erase
        (find
         (new_projectiles.begin(),
          new_projectiles.end(),
          id)); }

    bool isnew(const string id) {
      return count(new_projectiles.begin(), new_projectiles.end(), id); }

    /// Update the projectile and return a message to use to update
    /// the state.  "" means no message.  (erase == true) implies that
    /// the projectile should be deleted from the simulation
    string update(const string id, Projectile &p, bool &erase,
                  string &extra) {
      //cerr << "*.update:" << id << endl;
      erase = false;
      Point old(p.x, p.y);
      if (isnew(id)) { notnew(id); return helper::msg_new(id, p.x, p.y); }
      if (irrelevant(p)) { erase = true; return helper::msg_delete(id); }
      // Update projectile
      p.accelerate(state.global->wind, state.global->gravity);
      p.move();
      Point new_(p.x, p.y);
      Point hit = new_;
      bool has_hit = find_hit(map, old, new_, hit);
      if (has_hit) {
        //cerr << "hit" << endl;
        erase = true;
        /// TODO calculate the radius from some property of the
        /// projectile
        extra = helper::msg_delete(id);
        return helper::msg_explode(hit.x, hit.y, 50); }
      return helper::msg_move(id, new_.x, new_.y); }

    /// Update all projectiles and return a sequence of message for
    /// the interpreter to update the state.
    vector <string> move() {
      //cerr << "*.move:" << endl;
      //cerr << "projs" << endl;
      //FOREACH (Projectiles, it, projectiles)
      //cerr << "  " << it->first << endl;
      //cerr << "new_projs" << endl;
      //FOREACH (vector <Simulation::id>, it, new_projectiles)
      //cerr << "  " << *it << endl;
      vector <string> erase_after_loop;
      vector <string> messages;
      if (playerMoved) {
        game::Object &player = state.player();
        messages.push_back(helper::msg_move(player.id, player.x, player.y));
        playerMoved = false; }
      FOREACH (Projectiles, it, projectiles) {
        id id(it->first);
        Projectile &p(it->second);
        bool erase;
        string extra;
        string msg = update(id, p, erase, extra);
        if (msg.size()) messages.push_back(msg);
        if (extra.size()) messages.push_back(extra);
        if (erase) erase_after_loop.push_back(id); }
      FOREACH (vector<string>, it, erase_after_loop) {
        // cerr << "erasing: " << *it << endl;
        projectiles.erase(*it);
      }
      return messages; }};

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
        sim.set(id, player.x, player.y, dx, dy); }

      if (!command.compare("move")) {
        float dx, dy;
        i >> dx >> dy;
        /// @TODO hack!!!
        player.x += dx;
        player.y += dy;
        sim.playerMoved = true; }}};}
