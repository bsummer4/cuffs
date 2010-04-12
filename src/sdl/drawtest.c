#include "draw.h"

int main(void) {
	int ii = 1, jj = 1;
	sdl_init();
	while (ii++) {
		if (!(ii%10)) jj++;
		draw_circle((ii*4)%11-(ii*3)%9, jj%255, ii%255, ii%64+ii%60,
		            ii%800 + (jj % 9), (jj+ii)%600);
		flip(); }
	return 0; }
