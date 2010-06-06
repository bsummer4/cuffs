#include <SDL/SDL.h>
#include <tcl.h>
#include "keys.h"
#include "draw.h"

void Sdl_DoOneEvent() {
	SDL_Event e;
	if (!SDL_PollEvent(&e)) return;
	on_input_event(&e); }

void do_one_event () {
	Sdl_DoOneEvent();
	Tcl_DoOneEvent(TCL_DONT_WAIT); }

void entloop_ () {
	while (1) do_one_event(); }

static inline char *keyname (SDLKey k) { return SDL_GetKeyName(k); }
#include "macro.h"
static void print_stat (sdl_input_state *s) {
	printf("keys {");
	FORII (s->downkeys) {
		char *name = keyname(s->keys[ii]);
		bool last = ii==s->downkeys-1;
		bool quote = strchr(name, ' ');
		printf("%s%s%s%s", (quote?"{":""), name, (quote?"}":""), (last?"":" ")); }
	puts("}"); }

void hai_init () {
	keys_init(print_stat);
	sdl_init(); }
