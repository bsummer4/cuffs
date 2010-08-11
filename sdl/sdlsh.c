#include <stdbool.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <tcl.h>
#include <err.h>
#include <unistd.h>
#include <stdarg.h>
#include "keys.h"
#include "draw.h"
#include "macro.h"

static int Sdl_DoOneEvent () {
	SDL_Event e;
	int r;
	if ((r = SDL_PollEvent(&e)))
		on_input_event(&e);
	return r; }

static char *buttonname (uint8_t b) {
   static char *bs[] = {"mouse-left", "mouse-middle", "mouse-right", "mouse-4"};
   return (b<5)?bs[b-1]:"mouse-?"; }

// # Tcl setup

#define NCMDS 11
enum { FLIP=0, WHITE, LINE, RECT, CIRCLE, ARROW, SPRITE, IMAGE, SOUND, ENTLOOP,
       HAI };

static char *cmds[NCMDS] = {"flip", "white", "line", "rect", "circle",
                            "arrow", "sprite", "image", "sound", "entloop",
                            "hai"};

static int DrawOn (ClientData d, Tcl_Interp *i, int objc, Tcl_Obj *CONST objv[]) {
	Tcl_CmdInfo info;
	switch (objc) {
	case 2: {
		int len;
		char *cmd = Tcl_GetStringFromObj(objv[1], &len);
		Tcl_GetCommandInfo(i, cmd, &info);
		draw_on_begin((int)info.objClientData);
		break; }
	case 1: draw_on_end(); break;
	default: return TCL_ERROR; }
	return TCL_OK; }

static int Play (ClientData d, Tcl_Interp *i, int objc, Tcl_Obj *CONST objv[]) {
	int id=(int)d;
	play(id);
	return TCL_OK; }

static int DrawImg (ClientData d, Tcl_Interp *i, int objc, Tcl_Obj *CONST objv[]) {
	int x, y, id=(int)d;
	bool centered=false;
	switch (objc) {
	case 4: {
		int len;
		char *s = Tcl_GetStringFromObj(objv[1], &len);
		if (strcmp(s, "-centered") && strcmp(s, "-c")) return TCL_ERROR;
 		objv++;
		centered=true; }
	case 3:
		Tcl_GetIntFromObj(i, objv[1], &x);
		Tcl_GetIntFromObj(i, objv[2], &y);
		break;
	default:
		return TCL_ERROR; }
	if (centered) draw_center(id, x, y);
	else draw(id, x, y);
	return TCL_OK; }

#define EF(X) if (TCL_OK != X) errx(1, "fail fail fail")
static bool get_ints (Tcl_Interp *i, int *out, Tcl_Obj *CONST os[], int n, ...) {
	va_list argp;
	va_start(argp, n);
	for (; n>0; n--) {
		int count = va_arg(argp, int);
		if (count < 1) errx(1, "Bad arguments to get_ints");
		if (count == 1) EF(Tcl_GetIntFromObj(i, *os, out++));
		if (count > 1) {
			int len;
			Tcl_Obj **list;
			EF(Tcl_ListObjGetElements(i, *os, &len, &list));
			if (len != count) errx(1, "fail args");
			for (int ii=0; ii < len; ii++)
				EF(Tcl_GetIntFromObj(i, list[ii], out++)); }
		os++; }
	va_end(argp);
	return true; }

// TODO This is a mess.  Find ways to clean it up!
#define GRAB(n, ...) get_ints(i, p, objv+1, n, __VA_ARGS__)
static int Draw (ClientData d, Tcl_Interp *i, int objc, Tcl_Obj *CONST objv[]) {
	bool spritep=false;
	int p[9];
	switch ((int)d) {
	case FLIP: flip(); break;
	case WHITE: white(); break;
	case LINE:
		GRAB(4,  1, 4, 2, 2);
		draw_line(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8]);
		break;
	case RECT:
		GRAB(3,  2, 2, 4);
		draw_rect(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
		break;
	case CIRCLE:
		GRAB(3,  1, 2, 4);
		draw_circle(p[0], p[1], p[2], p[3], p[4], p[5], p[6]);
		break;
	case ARROW: {
		GRAB(3,  4, 2, 2);
		draw_arrow(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
		break; }
	case SOUND: {
		if (objc != 3) return TCL_ERROR;
		char *proc = Tcl_GetStringFromObj(objv[1], NULL);
		char *file = Tcl_GetStringFromObj(objv[2], NULL);
		int id = sound(file);
		if (!id) { warn("Couldn't load sound '%s'", file); return TCL_ERROR; }
		// TODO deleteProc destroy should release sound
		Tcl_CreateObjCommand(i, proc, Play, (ClientData)id, NULL);
		break; }
	case SPRITE: spritep = true;
	case IMAGE: {
		if (objc != 3) return TCL_ERROR;
		char *proc = Tcl_GetStringFromObj(objv[1], NULL);
		char *file = Tcl_GetStringFromObj(objv[2], NULL);
		int id = (spritep)?sprite(file):image(file);
		if (!id) { warn("Couldn't load image '%s'", file); return TCL_ERROR; }
		// TODO deleteProc destroy should release surface
		Tcl_CreateObjCommand(i, proc, DrawImg, (ClientData)id, NULL);
		break; }
	case ENTLOOP:
		for (;;)
			if (!Sdl_DoOneEvent() && !Tcl_DoOneEvent(TCL_DONT_WAIT))
				// Try not to poll too much; this seems to help.
				usleep(333);
	case HAI: {
		unsigned ii = 1, jj = 1;
		while ((ii++ < 12000)) {
			if (!(ii%10)) jj++;
			draw_circle((ii*4)%11-(ii*3)%9, (ii%800 + (jj % 9)), ((jj+ii)%600),
                     jj%255, ii%255, ii%64+ii%60, 200);
			if (ii%32 == 1) flip(); }
		break; }}
	return TCL_OK; }

static Tcl_Interp *main_interp;

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

int AppInit (Tcl_Interp *i) {
	main_interp = i;
	if (Tcl_Init(i) == TCL_ERROR) return TCL_ERROR;
	keys_init(handle_state);
	sdl_init(800, 600, 32, "TODO Make this an option");
	for (int ii=0; ii<NCMDS; ii++)
		Tcl_CreateObjCommand(i, cmds[ii], Draw, (ClientData)ii, NULL);
	Tcl_CreateObjCommand(i, "draw-on", DrawOn, NULL, NULL);
	Tcl_EvalFile(i, "./sdlsh.init");
	return TCL_OK; }

int main (int argc, char *argv[]) {
	Tcl_Main(argc, argv, AppInit); return 0; }
