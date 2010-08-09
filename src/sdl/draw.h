/*
	Some simple sdl drawing primitives for our game.

	Depends on <SDL/SDL_image.h>, and <SDL/SDL_mixer.h>
*/

#define MAX_SURFACES 100
#define MAX_SOUNDS 100

typedef SDL_Surface *surface;
typedef Mix_Chunk *chunk;
typedef unsigned ImageId;
typedef unsigned SoundId;
#define INVALID_ID 0

int sdl_init (void);
void draw_init (surface);
void flip ();
void white ();
void draw_circle (int radius,
                  int red, int green, int blue,
                  int x, int y);
