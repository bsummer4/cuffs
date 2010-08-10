// Implementation for draw.h

#include <math.h>
#include <stdbool.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_gfxPrimitives.h>
#include "draw.h"
#include <assert.h>
#include "macro.h"
#include <err.h>

// A quick, one-off vector library
typedef struct { double x, y; } V2;
typedef struct { int x, y; } V2i_t;
#define ST static inline
ST bool v2eq (V2 a, V2 b) { return a.x==b.x && a.y==b.y; }
ST V2 v2neg (V2 v) { return (V2){-v.x, -v.y}; }
ST V2 v2sub (V2 a, V2 b) { return (V2){a.x-b.x, a.y-b.y}; }
ST V2 v2add (V2 a, V2 b) { return (V2){a.x+b.x, a.y+b.y}; }
ST V2 v2mul (V2 v, double d) { return (V2){v.x*d, v.y*d}; }
ST V2 v2div (V2 v, double d) { return v2mul(v, 1/d); }
ST V2 v2floor (V2 v) { return (V2){floor(v.x), floor(v.y)}; }
ST double v2norm (V2 v) { return hypot(v.x, v.y); }
ST V2 v2normalized (V2 v) { return v2div(v, v2norm(v)); }
ST V2 v2perp (V2 v) { return (V2){-v.y, v.x}; }
ST V2i_t v2lround (V2 v) { return (V2i_t){lround(v.x), lround(v.y)}; }

static surface surfaces[MAX_SURFACES];
static chunk sounds[MAX_SOUNDS];
static surface screen;
static unsigned next_surface = 1; // 0 is INVALID_ID
static unsigned next_sound = 1; // 0 is INVALID_ID

void make_white_undrawn (surface surface) {
	const Uint32 white = SDL_MapRGB(surface->format, 255, 255, 255);
	SDL_SetColorKey(surface, SDL_SRCCOLORKEY, white); }

// Frees the old surface and replaces it with an optimized version
static void optimize (surface *s) {
	surface old = *s;
	surface result = SDL_DisplayFormat(old);
	if (!result) errx(1, "Couldn't optimize image.  ");
	SDL_FreeSurface(old);
	*s = result; }

static surface load_image (const char *filename) {
	surface result = IMG_Load(filename);
	if (result) optimize(&result);
	return result; }

static chunk load_sound (const char *filename) {
	chunk result = Mix_LoadWAV(filename);
	if (!result) errx(1, "Couldn't load sound.  ");
	return result; }

static void apply (int x, int y, surface from, surface dest) {
	SDL_Rect offset = {x, y};
	assert(!SDL_BlitSurface(from, NULL, dest, &offset)); }

static surface get_surface (ImageId id) {
	if (!id || id >= next_surface) errx(1, "Invalid image id: %u", id);
	return surfaces[id]; }

static chunk get_chunk (SoundId id) {
	if (!id || id >= next_sound) errx(1, "Invalid sound id: %u", id);
	return sounds[id]; }

// Public Interface
void flip () { SDL_Flip(screen); }

ImageId image (const char *filename) {
	if (next_surface > MAX_SURFACES) return INVALID_ID;
	surfaces[next_surface] = load_image(filename);
	if (!surfaces[next_surface]) return INVALID_ID;
	return next_surface++; }

ImageId sprite (const char *filename) {
	int r = image (filename);
	make_white_undrawn(surfaces[r]);
	return r; }

SoundId sound (const char *filename) {
	if (next_sound > MAX_SOUNDS) return INVALID_ID;
	sounds[next_sound] = load_sound(filename);
	if (!sounds[next_sound]) return INVALID_ID;
	return next_sound++; }

void draw_center (ImageId id, int x, int y) {
	surface image = get_surface(id);
	x -= image->w / 2;
	y -= image->h / 2;
	apply(x, y, image, screen); }

void draw (ImageId id, int x, int y) {
	surface s = get_surface(id);
	if (!s) errx(1, "No such image");
	apply(x, y, s, screen); }

void play (int id) {
	chunk c = get_chunk(id);
	if (-1 == Mix_PlayChannel(-1, c, 0))
		fprintf(stderr, "Warning: Unable to play sound: %d\n", id); }

void white () {
	boxRGBA(screen, 0, 0, screen->w, screen->h,
	        255, 255, 255, 255); }

void draw_line (int width, int red, int green, int blue, int alpha,
                int x0, int y0, int x1, int y1) {
	ITER (offset, -width, width + 1)
		lineRGBA (screen, MAX(0, x0 + offset), y0,
		          MAX(0, x1 + offset), y1,
		          red, green, blue, alpha); }

void draw_circle (int radius, int x, int y,
                  int red, int green, int blue, int alpha) {
	filledCircleRGBA(screen, x, y, radius, red, green, blue, alpha); }

void draw_rect (int x0, int y0, int x1, int y1, int red, int green,
                int blue, int alpha){
	boxRGBA(screen, x0, y0, x1, y1, red, green, blue, alpha); }

void draw_arrow (int red, int green, int blue, int alpha,
                 int x0, int y0, int x1, int y1) {
	V2 from = {x0, y0};
	V2 to = {x1, y1};
	V2 translation = v2sub(from, to);
	double length = v2norm(translation);
	V2 perp_trans = v2mul(v2normalized(v2perp(translation)), length/6);
	V2 base_minus = v2sub(from, perp_trans);
	V2 base_plus = v2add(from, perp_trans);
	filledTrigonRGBA(screen,
	                 to.x, to.y,
	                 base_plus.x, base_plus.y,
	                 base_minus.x, base_minus.y,
	                 red, green, blue, alpha); }

int sdl_init (int h, int w, int bpp, char *caption) {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	SDL_EnableKeyRepeat(60, 60);
	surface s = SDL_SetVideoMode(h, w, bpp, SDL_HWSURFACE|SDL_DOUBLEBUF);
	SDL_ShowCursor(SDL_DISABLE);
	SDL_WM_SetCaption(caption, NULL);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 256);
	screen = s;
	return 0; }

void sdl_die () { Mix_CloseAudio(); }
