#include <math.h>
#include <stdbool.h>
#include "vector2.h"

double v2norm (V2 v) { return hypot(v.x, v.y); }
V2 v2normalized (V2 v) { return v2div(v, v2norm(v)); }
V2 v2perp (V2 v) { return (V2){-v.y, v.x}; }
bool v2eq (V2 a, V2 b) { return a.x==b.x && a.y==b.y; }
V2 v2neg (V2 v) { return (V2){-v.x, -v.y}; }
V2 v2sub (V2 a, V2 b) { return (V2){a.x-b.x, a.y-b.y}; }
V2 v2add (V2 a, V2 b) { return (V2){a.x+b.x, a.y+b.y}; }
V2 v2mul (V2 v, double d) { return (V2){v.x*d, v.y*d}; }
V2 v2div (V2 v, double d) { return v2mul(v, 1/d); }
V2 v2floor (V2 v) { return (V2){floor(v.x), floor(v.y)}; }
V2i_t v2lround (V2 v) { return (V2i_t){lround(v.x), lround(v.y)}; }
