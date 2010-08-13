// Implementation for draw.h

#include <math.h>
#include <stdbool.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <assert.h>
#include <err.h>
#include "draw.h"
#include "macro.h"

typedef SDL_Surface *Surface;
typedef Mix_Chunk *Chunk;
typedef struct point point;

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

// Surface and sound ids start at 1 because 0 is reserved as an error
// code.
static Surface surfaces[MAX_SURFACES];
static Chunk sounds[MAX_SOUNDS];
static Surface screen;
static unsigned next_surface = 1;
static unsigned next_sound = 1;

static void make_white_undrawn (Surface s) {
	const Uint32 white = SDL_MapRGB(s->format, 255, 255, 255);
	SDL_SetColorKey(s, SDL_SRCCOLORKEY, white); }

// Frees the old surface and replaces it with an optimized version.
static void optimize (Surface *s) {
	Surface old=*s, result=SDL_DisplayFormat(old);
	if (!result) errx(1, "Couldn't optimize image.  ");
	SDL_FreeSurface(old);
	*s = result; }

static Surface load_image (const char *filename) {
	Surface result = IMG_Load(filename);
	if (result) optimize(&result);
	return result; }

static Chunk load_sound (const char *filename) {
	Chunk result = Mix_LoadWAV(filename);
	if (!result) errx(1, "Couldn't load sound.  ");
	return result; }

static void apply (point p, Surface from, Surface dest) {
	SDL_Rect offset = {p.x, p.y};
	assert(!SDL_BlitSurface(from, NULL, dest, &offset)); }

static Surface get_surface (unsigned id) {
	if (!id || id >= next_surface) errx(1, "Invalid image id: %u", id);
	return surfaces[id]; }

static Chunk get_chunk (unsigned id) {
	if (!id || id >= next_sound) errx(1, "Invalid sound id: %u", id);
	return sounds[id]; }

void flip () { SDL_Flip(screen); }

unsigned image (const char *filename) {
	if (next_surface > MAX_SURFACES) return 0;
	surfaces[next_surface] = load_image(filename);
	if (!surfaces[next_surface]) return 0;
	return next_surface++; }

unsigned sprite (const char *filename) {
	int r = image (filename);
	make_white_undrawn(surfaces[r]);
	return r; }

unsigned sound (const char *filename) {
	if (next_sound > MAX_SOUNDS) return 0;
	sounds[next_sound] = load_sound(filename);
	if (!sounds[next_sound]) return 0;
	return next_sound++; }

void draw_centered (unsigned id, point p) {
	Surface image = get_surface(id);
	p.x -= image->w / 2;
	p.y -= image->h / 2;
	apply(p, image, screen); }

void draw (unsigned id, point p) {
	Surface s = get_surface(id);
	if (!s) errx(1, "No such image");
	apply(p, s, screen); }

void play (unsigned id) {
	if (-1 == Mix_PlayChannel(-1, get_chunk(id), 0))
		warnx("Warning: Unable to play sound: %u", id); }

void white () {
	boxRGBA(screen, 0, 0, screen->w, screen->h,
	        255, 255, 255, 255); }

void drawline (point p1, point p2, int width, uint32_t color) {
	ITER (offset, -width, width + 1)
		lineColor (screen, MAX(0, p1.x + offset), p1.y,
		          MAX(0, p2.x + offset), p2.y, color); }

void drawcircle (int radius, point c, uint32_t color) {
	filledCircleColor(screen, c.x, c.y, radius, color); }

void drawrect (point p1, point p2, uint32_t color) {
	boxColor(screen, p1.x, p1.y, p2.x, p2.y, color); }

void drawarrow (point p1, point p2, uint32_t color) {
	V2 from={p1.x, p1.y}, to={p2.x, p2.y};
	V2 translation = v2sub(from, to);
	double length = v2norm(translation);
	V2 perp_trans = v2mul(v2normalized(v2perp(translation)), length/6);
	V2 base_minus = v2sub(from, perp_trans);
	V2 base_plus = v2add(from, perp_trans);
	filledTrigonColor(screen, to.x, to.y,
	                  base_plus.x, base_plus.y,
	                  base_minus.x, base_minus.y,
	                  color); }

static void drawon (Surface s) {
	static Surface save = NULL;
	if (s) save = screen, screen = s;
	else screen = save; }

void drawon_begin (unsigned id) { drawon(get_surface(id)); }
void drawon_end () { drawon NULL; }

int sdlinit (int h, int w, int bpp, char *caption) {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	SDL_EnableKeyRepeat(60, 60);
	Surface s = SDL_SetVideoMode(h, w, bpp, SDL_HWSURFACE|SDL_DOUBLEBUF);
	SDL_ShowCursor(SDL_DISABLE);
	SDL_WM_SetCaption(caption, NULL);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 256);
	screen = s;
	return 0; }
