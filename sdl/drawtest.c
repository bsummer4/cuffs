/*
	Creates a SDL window, Draws a bunch of circles in a cool-looking
	way until the screen is full, then exits.  Then end result looks
	like a bunch of bloby lines that move along various diagonals
	until the screen is full.
*/

#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include "draw.h"
#include <unistd.h>

int main (void) {
	unsigned ii = 1, jj = 1;
	sdl_init();
	while ((ii++ < 12000)) {
		if (!(ii%10)) jj++;
		draw_circle((ii*4)%11-(ii*3)%9, jj%255, ii%255, ii%64+ii%60,
		            ii%800 + (jj % 9), (jj+ii)%600);
		if (ii%8 == 1) flip(); }
	sleep(1);
	return 0; }
