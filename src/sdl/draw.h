#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_gfxPrimitives.h>

#define MAX_SURFACES 100
#define MAX_SOUNDS 100
#define INVALID 0
typedef SDL_Surface *surface;
typedef Mix_Chunk *chunk;
typedef unsigned ImageId;
typedef unsigned SoundId;

int sdl_init (void);
void draw_init (surface);
void flip ();
void white ();
void draw_circle (int radius,
                  int red, int green, int blue,
                  int x, int y);
