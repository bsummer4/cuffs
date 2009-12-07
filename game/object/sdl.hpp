#pragma once
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <cstdlib>
#include <boost/function.hpp>
#include "macro.h"

namespace sdl {
  using namespace std;
  typedef boost::function <void (SDL_Event&)> EventHandler;

  /// Although the SDL stuff is global, this is a wrapper to make it
  /// look like an object
  class SDL {
    vector <EventHandler> _event_handlers;
    bool _video_initialized;
    bool _audio_initialized;
  public:
    const bool video;
    const bool audio;
    SDL_Surface *screen;

    SDL (bool video=false, bool audio=false)
      : _video_initialized(false), _audio_initialized(false), 
        video(video), audio(audio), screen(NULL){
      Uint32 flags = 0 | SDL_INIT_TIMER;
      if (video) flags = (flags | SDL_INIT_VIDEO);
      if (audio) flags = (flags | SDL_INIT_AUDIO);
      if (SDL_Init(flags) == -1)
        throw runtime_error("Unable to initialize SDL"); }

    void registerEventHandler(EventHandler h) {
      _event_handlers.push_back(h); }

    /// Run until we see and event with type SDL_QUIT
    void runEventLoop() {
      SDL_Event event;
      LOOP {
        while (SDL_PollEvent (&event))
          if (event.type == SDL_QUIT) return;
          else
            FOREACH (vector <EventHandler>, handler, _event_handlers)
              (*handler)(event); }}

    bool video_initialized() { return _video_initialized; }
    bool audio_initialized() { return _audio_initialized; }
    void initVideo (int height, int width, int bitsperpixel,
                    string window_caption) {
      if (!video) throw runtime_error("SDL Wasn't initialized for video.  ");
      if (_video_initialized)
        throw runtime_error("SDL video initialized twice.  ");
      screen = SDL_SetVideoMode(height, width, bitsperpixel,
                                SDL_HWSURFACE|SDL_DOUBLEBUF);
      if (!screen) throw runtime_error(SDL_GetError());
      SDL_WM_SetCaption(window_caption.c_str(), NULL);
      _video_initialized = true; }

    void initAudio() {
      if (!audio) throw runtime_error("SDL Wasn't initialized for audio.  ");
      if (_audio_initialized)
        throw runtime_error("SDL audio initialized twice.  ");
      Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
      _audio_initialized = true; }

    ~SDL() { SDL_Quit(); Mix_CloseAudio(); }};


  // Frees the old surface and replaces it with an optimized version
  void optimize(SDL_Surface **surface, SDL &s) {
    assert(s.video_initialized());
    SDL_Surface *old = *surface;
    SDL_Surface *result = SDL_DisplayFormat(old);
    if (!result) throw runtime_error("Couldn't optimize image.  ");
    SDL_FreeSurface(old);
    *surface = result; }

  void make_white_undrawn(SDL_Surface* surface, SDL &s) {
    const Uint32 white = SDL_MapRGB(surface->format, 255, 255, 255);
    SDL_SetColorKey(surface, SDL_SRCCOLORKEY, white); }

  SDL_Surface *load_image_(string filename, SDL &s) {
    SDL_Surface *result = IMG_Load(filename.c_str());
    if (!result) throw runtime_error("Couldn't load image.  ");
    if (s.video_initialized()) optimize(&result, s);
    return result; }

  SDL_Surface *load_image(string filename, SDL &s) {
    SDL_Surface *result = IMG_Load(filename.c_str());
    if (!result) throw runtime_error("Couldn't load image.  ");
    if (s.video_initialized()) optimize(&result, s);
    make_white_undrawn(result, s);
    return result; }

  void applySurface(int x, int y,
                    SDL_Surface *source, SDL_Surface *destination) {
    SDL_Rect offset = {x, y};
    assert(!SDL_BlitSurface(source, NULL, destination, &offset)); }

  /// TODO This only works with 32bit images.
  Uint32 get_pixel(SDL_Surface *surface, int index) {
    SDL_PixelFormat* fmt = surface->format;
    SDL_LockSurface(surface);
    Uint32 result;
    if (fmt->BitsPerPixel == 32)
      result = ((Uint32*)surface->pixels)[index];
    else if (fmt->BitsPerPixel == 8) {
      Uint8 palette_index = ((Uint8*) surface->pixels)[index];
      SDL_Color color = fmt->palette->colors[palette_index];
      result = SDL_MapRGB(fmt, color.r, color.g, color.b);
      assert(false && "This doesn't work.  I don't know why.  "); }
    else assert(false);
    SDL_UnlockSurface(surface);
    return result; }

  Uint32 surface_pixel_grey_color(SDL_Surface *surface, int index) {
    Uint8 red, green, blue;
    Uint32 pix = get_pixel(surface, index);
    SDL_GetRGB(pix, surface->format, &red, &green, &blue);
    /// If it's grey, then all the colors should be the same.  So we
    /// just pick an arbitrary one
    return red; }

  /// A simple rendering object.  We initialize and quit sdl in our
  /// constructor and destructor.
  class Renderer {
  public:
    /// We expect that sdl.initVideo() has already been called.
    Renderer(SDL &sdl) : sdl(sdl) {};
    ~Renderer() {
      typedef map <string, SDL_Surface *> mss;
      FOREACH (mss, it, images) {
        delete it->second;
        images.erase(it); }}

    

    SDL_Surface *new_image(string id, string filename) {
      return (images[id] = load_image(filename, sdl)); }

    Mix_Chunk *new_sound(string id, string filename) {
      return (sounds[id] = Mix_LoadWAV(filename.c_str())); }

    void flip() { SDL_Flip(sdl.screen); }
    void draw_center(string id, int x, int y) {
      if (!images.count(id)) throw runtime_error("No such image");
      SDL_Surface *image = images[id];
      x -= image->w / 2;
      y -= image->h / 2;
      applySurface(x, y, image, sdl.screen); }
    void draw(string id, int x, int y) {
      if (!images.count(id)) throw runtime_error("No such image");
      applySurface(x, y, images[id], sdl.screen); }
    void play(string id) {
      if (!sounds.count(id)) throw runtime_error("No such sound");
      if (-1 != Mix_PlayChannel(-1, sounds[id], 0)) 
          cerr << "Warning: Unable to play sound:" << id << endl; } 
    void white() {
      boxRGBA(sdl.screen, 0, 0, sdl.screen->w, sdl.screen->h, 255, 255, 255, 255); }
    void draw_line(int width, int red, int green, int blue,
                   int x0, int y0, int x1, int y1) {
      // cerr << "draw_line" << width << red << green << blue << x0 << y0 << x1 << y1 << endl;
      ITER (offset, -width, width + 1)
        lineRGBA (sdl.screen, MAX(0, x0 + offset), y0,
                   MAX(0, x1 + offset), y1,
                   red, green, blue, 255); }

    void draw_circle(int radius, int red, int green, int blue,
                   int x, int y){
      filledCircleRGBA(sdl.screen, x, y, radius,
                 red, green, blue, 255); }

    void handleEvent(std::string event) {
      // cerr << " [render]-> " << event << endl;
      istringstream in(event);
      string command;
      in >> command;
      if (!command.compare("flip")) flip();
      if (!command.compare("image")) {
        string id, filename;
        in >> id >> filename;
        new_image(id, filename); }
      if (!command.compare("draw-center")) {
        string id; int x, y;
        in >> id >> x >> y;
        draw_center(id, x, y); };
      if (!command.compare("draw")) {
        string id; int x, y;
        in >> id >> x >> y;
        draw(id, x, y); };
      if (!command.compare("line")) {
        int width, red, green, blue, x0, y0, x1, y1;
        in >> width >> red >> green >> blue >> x0 >> y0 >> x1 >> y1;
        draw_line(width, red, green, blue, x0, y0, x1, y1); }
      if (!command.compare("circle")) {
        int radius, red, green, blue, x0, y0;
        in >> radius >> red >> green >> blue >> x0 >> y0;
        draw_circle(radius, red, green, blue, x0, y0); }
      if (!command.compare("sound")) {
        string id, filename;
        in >> id >> filename;
        new_sound(id, filename);
        /* cerr << "loaded sound: " << id << endl; */ }
      if (!command.compare("play")) {
        string id;
        in >> id;
        play(id);
        /* cerr << "Played: " << id << endl; */ }}

    /// TODO This is a hack.  Don't use this unless you really know
    /// what's up
    void inject_image(string id, SDL_Surface *s) {
      images[id] = s; }

  private:
    SDL &sdl;
    map <string, SDL_Surface *> images;
    map <string, Mix_Chunk *> sounds; };}
