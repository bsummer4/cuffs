/*
	# Keys Test

	This program just tests the keyevent detection in keys.c.  Run it
	and it will bring up a window.	Whenever a key is pressed or
	released, a list of depresed keys is printed to stdout.

	## TODO Mouse Supoort

   Add supoprt for optional mouse position and button state output.
*/

#include <SDL/SDL.h>
#include <stdbool.h>
#include "keys.h"
#include <stdio.h>
#include "macro.h"

static inline char *keyname (SDLKey k) { return SDL_GetKeyName(k); }

static void print_state (struct sdl_input_state *s) {
	printf("keys {");
	FORII (s->downkeys) {
		char *name = keyname(s->keys[ii]);
		bool last = ii==s->downkeys-1;
		bool quote = strchr(name, ' ');
		printf("%s%s%s%s", (quote?"{":""), name, (quote?"}":""), (last?"":" ")); }
	puts("}"); }

static inline void event_loop () {
	SDL_Event e;
	while (SDL_WaitEvent(&e)) {
		if (e.type == SDL_QUIT) exit(0);
		on_input_event(&e); }}

int main (int argc, char **argv) {
	SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	SDL_SetVideoMode(40, 40, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
	SDL_WM_SetCaption("hai", NULL);
	keys_init(print_state);
	event_loop();
	return 0; }
