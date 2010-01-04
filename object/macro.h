/// @TODO Move the other macro to it's own header.  Macros mixed with
/// prototypes is problematic.
#pragma once
#define ITER(var, from, to) for (int var = from; var < to; var++)
#define FORII(to) for (int ii = 0; ii != to; ii++)
#define MAKE(type, number) (type*)malloc(sizeof(type)*number);
#define SIGN(X) ((X >= 0) ? 1 : -1)
#define MAX(x, y) ((x > y) ? x : y)
#define MIN(x, y) ((x < y) ? x : y)
#define ABS_MAX(X, Y) ((abs(X) > abs(Y)) ? X : Y)
#define ABS_MIN(X, Y) ((abs(X) < abs(Y)) ? X : Y)
#define LOOP while (true)
#define FOREACH(type, var, seq)         \
  for (type::iterator var(seq.begin()); \
       var != seq.end();                \
       ++var)
