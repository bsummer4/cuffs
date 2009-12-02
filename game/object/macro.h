/// @TODO Move the other macro to it's own header.  Macros mixed with
/// prototypes is problematic.
#pragma once
#undef iter
#define ITER(var, from, to) for (int var = from; var < to; var++)
#define FORII(to) for (int ii = 0; ii != to; ii++)
#define MAKE(type, number) (type*)malloc(sizeof(type)*number);
#define MAX(x, y) ((x > y) ? x : y)
#define MIN(x, y) ((x < y) ? x : y)
#define LOOP while (true)
#define FOREACH(type, var, seq)         \
  for (type::iterator var(seq.begin()); \
       var != seq.end();                \
       ++var)
