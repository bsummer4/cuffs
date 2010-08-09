/*
	A one-off vector library in C.

	Depends on <math.h> and <stdbool.h>
*/

typedef struct { double x, y; } V2;
typedef struct { int x, y; } V2i_t;

double v2norm (V2);
V2 v2normalized (V2);
V2 v2perp (V2);
bool v2eq (V2, V2);
V2 v2neg (V2);
V2 v2sub (V2, V2);
V2 v2add (V2, V2);
V2 v2div (V2, double);
V2 v2mul (V2, double);
V2 v2floor (V2);
V2i_t v2lround (V2);
