/*
	Some simple sdl drawing primitives for our game.  This also handles
	sound output.

	Depends on <SDL/SDL_image.h>, and <SDL/SDL_mixer.h>
*/

#define MAX_SURFACES 100
#define MAX_SOUNDS 100

typedef SDL_Surface *surface;
typedef Mix_Chunk *chunk;
typedef unsigned ImageId;
typedef unsigned SoundId;
#define INVALID_ID 0

int sdl_init (int height, int width, int bitsperpixel, char *caption);
void draw_init (surface);
void flip ();
void white ();
void draw_circle (int radius, int r, int g, int b, int x, int y);
void draw_line (int width, int r, int g, int b, int x0, int y0, int x1, int y1);
void draw_rect (int x0, int y0, int x1, int y1, int r, int g, int b, int a);
void draw_arrow (int r, int g, int b, int x0, int y0, int x1, int y1);

