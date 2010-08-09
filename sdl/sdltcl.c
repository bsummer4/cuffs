#include <stdbool.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <tcl.h>
#include <err.h>
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

#define NCMDS 12
enum { FLIP=0, WHITE, LINE, RECT, CIRCLE, ARROW, DRAW_CENTER, DRAW, PLAY,
       IMAGE, SOUND, ENTLOOP };

static char *cmds[NCMDS] = {"flip", "white", "line", "rect", "circ",
                            "arrow", "draw-center", "draw", "play",
                            "image", "sound", "entloop"};

static int Draw (ClientData d, Tcl_Interp *i, int objc, Tcl_Obj *CONST objv[]) {
	switch ((int)d) {
	case FLIP: flip(); break;
	case WHITE: white(); break;
	case LINE: {
		int p[8];
		for (int ii=0; ii < 8; ii++)
			Tcl_GetIntFromObj(i, objv[ii+1], p+ii);
		draw_line(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]); }
		break;
	case RECT: {
		int p[8];
		for (int ii=0; ii < 8; ii++)
			Tcl_GetIntFromObj(i, objv[ii+1], p+ii);
		draw_rect(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]); }
		break;
	case CIRCLE: {
		int p[6];
		for (int ii=0; ii < 6; ii++)
			Tcl_GetIntFromObj(i, objv[ii+1], p+ii);
		draw_circle(p[0], p[1], p[2], p[3], p[4], p[5]);
		break; }
	case ARROW: {
		int p[7];
		for (int ii=0; ii < 7; ii++)
			Tcl_GetIntFromObj(i, objv[ii+1], p+ii);
		draw_arrow(p[0], p[1], p[2], p[3], p[4], p[5], p[6]);
		break; }
	case ENTLOOP:
		for (;;) Sdl_DoOneEvent(), Tcl_DoOneEvent(TCL_DONT_WAIT); }
	return TCL_OK; }

int AppInit (Tcl_Interp *interp) {
	if (Tcl_Init(interp) == TCL_ERROR) return TCL_ERROR;
	keys_init(print_stat);
	sdl_init(800, 600, 32, "TODO Make this an option");
	for (int ii=0; ii<NCMDS; ii++)
		Tcl_CreateObjCommand(interp, cmds[ii], Draw, (ClientData)ii, NULL);
	Tcl_SetVar(interp, "tcl_rcFileName", "sdlgo.tcl", TCL_GLOBAL_ONLY);
	return TCL_OK; }

int main (int argc, char *argv[]) {
	Tcl_Main(argc, argv, AppInit); return 0; }

