L = -lSDL_image -lSDL_gfx -lSDL_mixer -lboost_thread
SDL=$(shell pkg-config --cflags sdl) $(L)
CFLAGS = --std=c99 -Wall -pedantic
TCL = -I/usr/include/tcl8.5 -L/usr/lib/tcl8.5 -ltcl -lm

all: tcl tests
tcl: sdl.so
tests: drawtest keytest tcl
test: tests; ./keytest && ./sdltest.tcl && ./drawtest
clean:; rm -f sdl_wrap.c *~ sdl.so drawtest keytest

drawtest: draw.c drawtest.c; g++ $^ $(SDL) -o $@
keytest: keys.c keytest.c; g++ $^ $(SDL) -o $@
sdl_wrap.c: sdl.i draw.c draw.h; swig -tcl $<
sdl.so: draw.c sdl_wrap.c; g++ $^ -shared $(TCL) $(SDL) $(CFLAGS) -o $@
