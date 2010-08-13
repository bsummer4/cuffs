/*
	Some simple sdl drawing and sound primitives.

	For all ids (unsigned), 0 is a failure code.
*/

#define MAX_SURFACES 100
#define MAX_SOUNDS 100

struct point { int x, y; };

int sdlinit (int height, int width, int bitsperpixel, char *windowcaption);
void flip ();
void white ();
void drawcircle (int radius, struct point center, uint32_t color);
void drawline (struct point p1, struct point p2, int width, uint32_t color);
void drawrect (struct point p1, struct point p2, uint32_t color);
void drawarrow (struct point base, struct point tip, uint32_t color);
void play (unsigned id);
void draw (unsigned id, struct point topleft);
void draw_centered (unsigned id, struct point center);
unsigned sprite (const char *filename);
unsigned image (const char *filename);
unsigned sound (const char *filename);

// Between these two calls, drawing happens on another image instead of on
// the screen.
void drawon_begin (unsigned id);
void drawon_end ();
