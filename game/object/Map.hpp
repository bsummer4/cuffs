#pragma once
#include <string>
#include <map>
#include <vector>
#include <set>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdexcept>
#include "sdl.hpp"
#include "vectors.hpp"

namespace game {
  using namespace std;
  using namespace vectors;
  typedef unsigned Team;
  enum { MAP_EMPTY=255, MAP_SOLID=0 };

  /// This handles reading a map from a file.  This is meant to be
  /// passed into the Map constructor and should not be used outside
  /// of that.
  ///
  /// WARNING: Because we want Map to reuse our data, heap-allocated
  /// data is *NOT FREED* in the destructor!!
  ///
  /// Example use:
  ///     Map m(MapLoader("example.map"));
  ///     cout << m.width << " " << m.map[m.width * m.height - 1] << endl;
  class MapLoader {
    sdl::SDL &sdl;
  public:
    SDL_Surface* map;
    set <pair <int, int> > spawn_points;

    MapLoader(const string &filename, sdl::SDL &sdl)
      : sdl(sdl) {
      ifstream in(filename.c_str());
      if (!in.is_open()) throw runtime_error("Invalid map file.  ");
      map = NULL;
      while (handle_line(in));
      if (!map) runtime_error("No image for map in file.  ");}

  private:
    bool handle_line(ifstream &in) {
      string command;
      if (!(in >> command)) return false;
      if (!command.compare("spawn")) {
        int x, y;
        if (!(in >> x >> y)) throw runtime_error("Invalid file");
        spawn_points.insert(make_pair<int, int>(x, y));
        return true; }
      if (!command.compare("map")) {
        string filename;
        if (map) throw runtime_error("Two images for map in file.  ");
        if (!(in >> filename)) throw runtime_error("Invalid file");
        map = sdl::load_image(filename, sdl);
        return true; }
      throw runtime_error("Invalid command in file"); }};

  // TODO We assume that pixels in the SDL_Surface are always ints.
  // This is not always true.  Make this more robust
  class Map {
  public:
    SDL_Surface* const map;
    Map(const MapLoader &l)
      : map(l.map), width(map->w), height(map->h), max_pixel(255),
        spawn_points(l.spawn_points) {}
    unsigned _(int x, int y) { return get_pixel(x, y); }
    unsigned get_pixel(int x, int y) {
      return sdl::surface_pixel_grey_color(map, point(x, y)); }

    void explosion(int center_x, int center_y, float radius) {
      if (radius <= 0) return;
      filledCircleRGBA(map, center_x, center_y, radius,
                       MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, 255); }

    const int width, height, max_pixel;
    const set <pair <int, int> > spawn_points;
    bool is_solid(int p) { return p != MAP_EMPTY; }
    bool is_solid(int x, int y) {
      if (y < 0 || y > height) return false;
      return is_solid(_(x, y)); }
    void wrap_point(int &x, int &y) {
      // Ignore Y because we don't wrap it.
      while (x < 0) x += width;
      x = (x % this->width); }
    void wrap_point(double &x, double &y) {
      // Ignore Y because we don't wrap it.
      while (x < 0) x += width;
      x = ((int)x % this->width) + (x - floor(x)); }

  private:
    int point(int x, int y) {
      wrap_point(x, y);
      return width * y + x; }};

  /// For testing
  void map_to_pgm(Map &map, ostream &out) {
    out << "P2 " << map.width << " " << map.height << " " << map.max_pixel;
    ITER (y, 0, map.height) {
      ITER (x, 0, map.width) {
        if (!x) out << "\n";
        unsigned pixel = map._(x, y);
        out << pixel;
        if (x != map.width-1) out << " "; }}
    out << endl; }}
