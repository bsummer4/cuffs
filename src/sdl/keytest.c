// # Temporary Testing Scaffolding
// g++ -Wall -Werror -lSDL keys.c keytest.c

#include <stdio.h>
#include <SDL/SDL.h>
#include "keys.h"
#include "macro.h"

static inline char *keyname (SDLKey k) { return SDL_GetKeyName(k); }

static void print_state (sdl_input_state *s) {
	printf("keys {");
	FORII (s->downkeys)
		printf("{%s == %d}%s",
		       keyname(s->keys[ii]), s->keys[ii],
		       ii==s->downkeys-1 ? "" : " ");
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
