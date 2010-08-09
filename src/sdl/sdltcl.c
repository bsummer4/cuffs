#include <stdbool.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <tcl.h>
#include "keys.h"
#include "draw.h"
#include "macro.h"

static void Sdl_DoOneEvent () {
	SDL_Event e;
	if (SDL_PollEvent(&e))
		on_input_event(&e); }

static void print_stat (struct sdl_input_state *s) {
	printf("keys {");
	FORII (s->downkeys) {
		char *name = SDL_GetKeyName(s->keys[ii]);
		bool last = ii==s->downkeys-1;
		bool quote = strchr(name, ' ');
		printf("%s%s%s%s", (quote?"{":""), name, (quote?"}":""), (last?"":" ")); }
	puts("}"); }

enum { FLIP=0, WHITE, DRAW_CIRCLE, HAI, ENTLOOP };

static int Draw (ClientData d, Tcl_Interp *i, int objc, Tcl_Obj *CONST objv[]) {
	switch ((int)d) {
	case FLIP: flip(); break;
	case WHITE: white(); break;
	case DRAW_CIRCLE: {
		int p[6];
		for (int ii=0; ii < 6; ii++)
			Tcl_GetIntFromObj(i, objv[ii+1], p+ii);
		draw_circle(p[0], p[1], p[2], p[3], p[4], p[5]);
		break; }
	case HAI:
		white();
		draw_circle(20, 64, 64, 64, 20, 20);
		flip();
		break;
	case ENTLOOP:
		for (;;) Sdl_DoOneEvent(), Tcl_DoOneEvent(TCL_DONT_WAIT); }
	return TCL_OK; }

static char *fns[5] = {"flip", "white", "draw_circle", "hai", "entloop"};
int AppInit (Tcl_Interp *interp) {
	if (Tcl_Init(interp) == TCL_ERROR) return TCL_ERROR;
	keys_init(print_stat);
	sdl_init();
	for (int ii=0; ii<5; ii++)
		Tcl_CreateObjCommand(interp, fns[ii], Draw, (ClientData)ii, NULL);
	Tcl_SetVar(interp, "tcl_rcFileName", "sdlgo.tcl", TCL_GLOBAL_ONLY);
	return TCL_OK; }

int main (int argc, char *argv[]) {
	Tcl_Main(argc, argv, AppInit); return 0; }
