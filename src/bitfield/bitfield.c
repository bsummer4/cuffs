#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include "bitfield.h"

static const int bits_per_unsigned = sizeof(unsigned) * 8;
static inline bool valid_bitfield (bitfield f) { return f.bits; }
static inline bool valid_index (bitfield f, int x, int y) {
	return (valid_bitfield(f)
	        && x >= 0 && y >= 0 && x < f.width && y < f.height); }

// Initialized to all false values.
bitfield bitfield_create (int width, int height) {
	char *mem = malloc(height * width);
	if (mem) bzero(mem, height * width);
	return (bitfield){height, width, mem}; }

void bitfield_uncreate (bitfield f) { if (f.bits) free(f.bits); }

// Nothing will happen if any of the inputs are invalid
void bitfield_set (bitfield f, int x, int y, bool value) {
	if (valid_index(f, x, y)) {
		// printf("set {f %d %d} %d\n", x, y, value);
		f.bits[x + y * f.width] = value; }}

// Returns -1 on invalid inputs
int bitfield_get (bitfield f, int x, int y) {
	return ((valid_index(f, x, y)) ? f.bits[x + y * f.width] : -1); }

// Return false if there were errors (for example there are no bits in the
// range specified).  'offset' is based on the length of 'unsigned's.
bool bitfield_extract (bitfield f, int offset, unsigned *output) {
	if (!valid_bitfield(f)) return false;
	const int num_bits = f.width * f.height;
	const int bit_offset = offset * bits_per_unsigned;
	const int bits_left = num_bits - bit_offset;
	if (bits_left <= 0) return false;

	*output = 0;
	unsigned current_bitvalue = 1;
	for (int ii = 0;
	     ii < bits_left && ii < bits_per_unsigned;
	     ii++, current_bitvalue *= 2) {
		if (f.bits[ii + bit_offset]) {
			*output |= current_bitvalue; }}
        return true; }
