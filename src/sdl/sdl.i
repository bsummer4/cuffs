%module sdl

%{
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include "draw.h"
#include "keys.h"
#include "tclloop.h"
static void hai() { draw_circle(20, 64, 64, 64, 20, 20); }
%}

%include draw.h
%include keys.h
%include tclloop.h
void hai();
