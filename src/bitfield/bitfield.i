%module bitfield

%{
#include "bitfield.h"
	bitfield *bitfield_new (int width, int height);
	void bitfield_delete (bitfield* f);
%}

%include bitfield.h

%inline %{
	static inline bool valid_bitfield (bitfield f) { return f.bits; }
	bitfield *bitfield_new (int width, int height) {
		bitfield *result = malloc(sizeof(bitfield));
		if (result)
			*result = bitfield_create(width, height);
		if (!result || valid_bitfield(*result))
			return result;
		bitfield_uncreate(*result);
		free(result);
		return NULL; }

	void bitfield_delete (bitfield* f) {
		if (f) {
			bitfield_uncreate(*f);
			free(f); }}
	%}
