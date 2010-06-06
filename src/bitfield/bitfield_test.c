#include "bitfield.h"
#include <stdio.h>

static inline void newline () { putc('\n', stdout); }

// puts "${width}x${height}x${big hex number}"
static inline void bitfield_serialize (bitfield f) {
	unsigned unsigneds[800];
	int used = 0;
	while (bitfield_extract(f, used, unsigneds + used)) used++;

	printf("%dx%dx", f.width, f.height);
	for (int ii = used - 1; ii >= 0; ii--)
		printf("%08x", unsigneds[ii]);
	newline(); }

int main (int argc, char **argv) {
	bitfield f = bitfield_create(32, 2);
	bitfield_set(f, 1, 1, true);
	bitfield_set(f, 31, 1, true);
	bitfield_set(f, 1, 0, true);
	bitfield_set(f, 2, 0, true);
	bitfield_serialize(f);
	bitfield_uncreate(f);

	// bitfield *fp = bitfield_new(32, 2);
	// bitfield_set(*fp, 1, 1, true);
	// bitfield_set(*fp, 31, 1, true);
	// bitfield_set(*fp, 1, 0, true);
	// bitfield_set(*fp, 2, 0, true);
	// bitfield_serialize(*fp);
	// bitfield_delete(fp);
}
