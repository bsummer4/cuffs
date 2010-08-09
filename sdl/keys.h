/*
	# SDL Input Listener

	A simple library for listening to SDL input events.

	Currently, we keep track of the mouse position, the depressed mouse
	buttons, and the depressed keyboard buttons.

   Depends on <SDL/SDL.h>
*/

#define MAX_PRESSED_KEYS 100

struct sdl_input_state {
	SDLKey keys[MAX_PRESSED_KEYS];
	int downkeys;
	Uint8 mousebuttons[5];
	int downbuttons;
	SDLMod mod;
	Uint16 mouse[2]; };

typedef void (*sdl_input_state_handler)(struct sdl_input_state *s);
void keys_init(sdl_input_state_handler);
void on_input_event(SDL_Event*);
