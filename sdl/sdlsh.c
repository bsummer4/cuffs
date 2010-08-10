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

static char *buttonname (uint8_t b) {
   static char *bs[] = {"mouse-left", "mouse-middle", "mouse-right", "mouse-4"};
   return (b<5)?bs[b-1]:"mouse-?"; }

// # Tcl setup

#define NCMDS 14
enum { FLIP=0, WHITE, LINE, RECT, CIRCLE, ARROW, DRAW_CENTER, DRAW, PLAY,
       SPRITE, IMAGE, SOUND, ENTLOOP, HAI };

static char *cmds[NCMDS] = {"flip", "white", "line", "rect", "circ",
                            "arrow", "draw-center", "draw", "play",
                            "sprite", "image", "sound", "entloop", "hai"};

// TODO This is a mess.  Find ways to clean it up!
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
	case DRAW_CENTER: {
		int p[3];
		for (int ii=0; ii < 3; ii++)
			Tcl_GetIntFromObj(i, objv[ii+1], p+ii);
		draw_center(p[0], p[1], p[2]);
		break; }
	case DRAW: {
		int p[3];
		for (int ii=0; ii < 3; ii++)
			Tcl_GetIntFromObj(i, objv[ii+1], p+ii);
		draw(p[0], p[1], p[2]);
		break; }
	case PLAY: {
		int id;
		Tcl_GetIntFromObj(i, objv[1], &id);
		play(id);
		break; }
	case SOUND: {
		char *x = Tcl_GetStringFromObj(objv[1], NULL);
		int id = sound(x);
		if (!id) { warn("Couldn't load sound '%s'", x); return TCL_ERROR; }
		Tcl_SetObjResult(i, Tcl_NewIntObj(id));
		break; }
	case SPRITE: {
		char *x = Tcl_GetStringFromObj(objv[1], NULL);
		int id = sprite(x);
		if (!id) { warn("Couldn't load image '%s'", x); return TCL_ERROR; }
		Tcl_SetObjResult(i, Tcl_NewIntObj(id));
		break; }
	case IMAGE: {
		char *x = Tcl_GetStringFromObj(objv[1], NULL);
		int id = image(x);
		if (!id) { warn("Couldn't load image '%s'", x); return TCL_ERROR; }
		Tcl_SetObjResult(i, Tcl_NewIntObj(id));
		break; }
	case ENTLOOP:
		for (;;) Sdl_DoOneEvent(), Tcl_DoOneEvent(TCL_DONT_WAIT);
	case HAI: {
		unsigned ii = 1, jj = 1;
		while ((ii++ < 12000)) {
			if (!(ii%10)) jj++;
			draw_circle((ii*4)%11-(ii*3)%9, jj%255, ii%255, ii%64+ii%60,
			            ii%800 + (jj % 9), (jj+ii)%600);
			if (ii%32 == 1) flip(); }
		break; }}
	return TCL_OK; }

static Tcl_Interp *main_interp;

// TODO: yuck!
static void handle_state (struct sdl_input_state *s) {
	int nkeys = s->downkeys + s->downbuttons;
	Tcl_Obj *keys[nkeys];
	for (int ii=0; ii<nkeys; ii++) {
		if (ii < s->downkeys)
			keys[ii] = Tcl_NewStringObj(SDL_GetKeyName(s->keys[ii]), -1);
		else
			keys[ii] = Tcl_NewStringObj(buttonname(ii-(s->downkeys)), -1); }
	Tcl_Obj *mouse[2] = {Tcl_NewIntObj(s->mouse[0]),
	                     Tcl_NewIntObj(s->mouse[1])};
	Tcl_Obj *keys_mouse[4] = {Tcl_NewStringObj("keys", -1),
	                          Tcl_NewListObj(nkeys, keys),
	                          Tcl_NewStringObj("mouse", -1),
	                          Tcl_NewListObj(2, mouse)};
	Tcl_Obj *r = Tcl_NewListObj(4, keys_mouse);
	Tcl_Obj *fuckthis[2] = {Tcl_NewStringObj("oninput", -1), r};
	Tcl_EvalObjEx(main_interp, Tcl_NewListObj(2, fuckthis), TCL_EVAL_GLOBAL); }

int AppInit (Tcl_Interp *interp) {
	main_interp = interp;
	if (Tcl_Init(interp) == TCL_ERROR) return TCL_ERROR;
	keys_init(handle_state);
	sdl_init(800, 600, 32, "TODO Make this an option");
	for (int ii=0; ii<NCMDS; ii++)
		Tcl_CreateObjCommand(interp, cmds[ii], Draw, (ClientData)ii, NULL);
	Tcl_SetVar(interp, "tcl_rcFileName", "sdlsh.init", TCL_GLOBAL_ONLY);
	return TCL_OK; }

int main (int argc, char *argv[]) {
	Tcl_Main(argc, argv, AppInit); return 0; }
