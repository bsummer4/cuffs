// # SDL Input Listener
// A simple library for listening to SDL input events.
//
// TODO Don't {} quote things that don't need to be {} quoted.

#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>
#include <SDL/SDL.h>
#include "keys.h"
#include "macro.h"

// Initialized at keys_init(callback).
static sdl_input_state_handler callback;
static struct sdl_input_state s;

template <typename T>
static inline int lsearch(T v, T *base, int max) {
	FORII (max) { if (v == base[ii]) return ii; }
	return -1; }

template <typename T>
static inline void add (T t, T *ts, int *used, int max) {
	int ii = lsearch (t, ts, *used);
	if (-1 == ii && *used < max) ts[(*used)++] = t; }

// Delete an element from an array
template <typename T>
static inline void del (T t, T *ts, int *used) {
	int ii = lsearch (t, ts, *used);
	if (-1 == ii) return;
	const int last = *used - 1;
	if (ii != last) ts[ii] = ts[last];
	(*used)--; }

static inline void add_key (SDLKey k) {
	add(k, s.keys, &s.downkeys, MAX_PRESSED_KEYS); }
static inline void add_mbutton (Uint8 b) {
	add(b, s.mousebuttons, &s.downbuttons, 5); }
static inline void del_key(SDLKey k) {
	del(k, s.keys, &s.downkeys); }
static inline void del_mbutton (Uint8 b) {
	del(b, s.mousebuttons, &s.downbuttons); }

static inline char *keyname (SDLKey k) { return SDL_GetKeyName(k); }

static inline void handle_keyevent (SDL_KeyboardEvent *e) {
	SDLKey k = e->keysym.sym;
	if (k == SDLK_ESCAPE) exit(0);
	if (e->type == SDL_KEYDOWN) add_key(k);
	else del_key(k);
	s.mod = e->keysym.mod; }

static inline void handle_mousebuttonevent (SDL_MouseButtonEvent *e) {
	if (e->type == SDL_MOUSEBUTTONDOWN)
		add_mbutton (e->button);
	else del_mbutton (e->button);
	s.mouse[0] = e->x; s.mouse[1] = e->y; }

static inline void handle_mousemotionevent(SDL_MouseMotionEvent *e) {
	s.mouse[0] = e->x; s.mouse[1] = e->y; }


// ## Public Interface
void keys_init(sdl_input_state_handler h) {
	callback = h;
	bzero(&s, sizeof(s));
	callback(&s); }

void on_input_event(SDL_Event *event) {
	static bool changed = false;
	switch (event->type) {
	case SDL_KEYUP:
	case SDL_KEYDOWN:
		changed = true;
		handle_keyevent(&event->key);
		break;
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
		changed = true;
		handle_mousebuttonevent(&event->button);
		break;
	case SDL_MOUSEMOTION:
		handle_mousemotionevent(&event->motion);
		break;
	default: break; }
	const bool just_changed = changed;
	changed = false;
	if (just_changed) callback(&s); }
