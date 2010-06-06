// This is a small library in c that implements a bitfield data
// structure.  It maintians a height, a width, and height * width
// bits.  Each bit can be retrived and set individually.  We can
// convert to a sequence of integers.

#include <stdbool.h>

// TODO Research different repersentations.  I know this way is easy
//      and it least gets us relatively fast lookups, but I don't know
//      if there is a better way.
typedef struct {
	unsigned height, width;
	char *bits; } bitfield;

extern bitfield bitfield_create (int width, int height);
extern void bitfield_uncreate (bitfield f);
extern int bitfield_get (bitfield f, int x, int y);
extern void bitfield_set (bitfield f, int x, int y, bool value);
extern bool bitfield_extract (bitfield f, int offset, unsigned *output);
