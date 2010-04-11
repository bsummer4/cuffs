// TODO

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <assert.h>
#include "macro.h"
#include "vectors.hpp"
using namespace vectors;

#define error(str) exit(1);
#define MAX_SURFACES 100
#define MAX_SOUNDS 100
#define INVALID 0
typedef SDL_Surface *surface;
typedef Mix_Chunk *chunk;
typedef unsigned ImageId;
typedef unsigned SoundId;

static surface surfaces[MAX_SURFACES];
static chunk sounds[MAX_SOUNDS];
static surface screen;
static unsigned next_surface = 1; // 0 is INVALID
static unsigned next_sound = 1; // 0 is INVALID

static inline void hide_cursor() { SDL_ShowCursor(SDL_DISABLE); }
static inline void make_white_undrawn(surface surface) {
	const Uint32 white = SDL_MapRGB(surface->format, 255, 255, 255);
	SDL_SetColorKey(surface, SDL_SRCCOLORKEY, white); }

// Frees the old surface and replaces it with an optimized version
static inline void optimize(surface *s) {
	surface old = *s;
	surface result = SDL_DisplayFormat(old);
	if (!result) error("Couldn't optimize image.  ");
	SDL_FreeSurface(old);
	*s = result; }

static inline surface load_image(const char *filename) {
	surface result = IMG_Load(filename);
	if (!result) error("Couldn't load image.  ");
	optimize(&result);
	return result; }

static inline chunk load_sound(const char *filename) {
	chunk result = Mix_LoadWAV(filename);
	if (!result) error("Couldn't load sound.  ");
	return result; }

static inline void apply (int x, int y, surface from, surface dest) {
	SDL_Rect offset = {x, y};
	assert(!SDL_BlitSurface(from, NULL, dest, &offset)); }

static inline surface get_surface(ImageId id) {
	if (!(id < 1 || id >= next_surface)) error("Invalid Image Id");
	return surfaces[id]; }

static inline chunk get_chunk (SoundId id) {
	if (!(id < 1 || id >= next_sound)) error("Invalid Sound Id");
	return sounds[id]; }

// Public Interface
void flip() { SDL_Flip(screen); }
void draw_init (surface screen_) { screen = screen_; }
ImageId image (const char *filename) {
	if (next_surface > MAX_SURFACES) return INVALID;
	surfaces[next_surface] = load_image(filename);
	if (!surfaces[next_surface]) return INVALID;
	return next_surface++; }

SoundId sound (const char *filename) {
	if (next_sound > MAX_SOUNDS) return INVALID;
	sounds[next_sound] = load_sound(filename);
	if (!sounds[next_sound]) return INVALID;
	return next_sound++; }

void draw_center(ImageId id, int x, int y) {
	surface image = get_surface(id);
	x -= image->w / 2;
	y -= image->h / 2;
	apply(x, y, image, screen); }

void draw(ImageId id, int x, int y) {
	surface s = get_surface(id);
	if (!s) error("No such image");
	apply(x, y, s, screen); }

void play(int id) {
	chunk c = get_chunk(id);
	if (-1 == Mix_PlayChannel(-1, c, 0))
		fprintf(stderr, "Warning: Unable to play sound: %d\n", id); }

void white() {
	boxRGBA(screen, 0, 0, screen->w, screen->h,
	        255, 255, 255, 255); }

void draw_line(int width, int red, int green, int blue,
               int x0, int y0, int x1, int y1) {
	ITER (offset, -width, width + 1)
		lineRGBA (screen, MAX(0, x0 + offset), y0,
		          MAX(0, x1 + offset), y1,
		          red, green, blue, 255); }

void draw_circle(int radius, int red, int green, int blue,
                 int x, int y){
	filledCircleRGBA(screen, x, y, radius,
	                 red, green, blue, 255); }

void draw_rect(int x0, int y0, int x1, int y1, int red, int green,
               int blue, int alpha){
	boxRGBA(screen, x0, y0, x1, y1, red, green, blue, alpha); }

void draw_arrow(int red, int green, int blue,
                int x0, int y0, int x1, int y1) {
	typedef Vector2 <double> V2;
	V2 from(x0, y0);
	V2 to(x1, y1);
	V2 translation(from - to);
	double length = translation.norm();
	V2 perp_trans = translation.perp().normalized() *  (length / 6);
	V2 base_minus = from - perp_trans;
	V2 base_plus = from + perp_trans;
	filledTrigonRGBA(screen,
	                 to.x, to.y,
	                 base_plus.x, base_plus.y,
	                 base_minus.x, base_minus.y,
	                 red, green, blue, 128); }

int main(void) {
	return 0; }
