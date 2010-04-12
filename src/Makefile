CFLAGS = --std=c99 -Wall -pedantic
TCL = -I/usr/include/tcl8.5 -L/usr/lib/tcl8.5 -ltcl -lm
all: test tcl
clean:; rm -f bitfield_test bitfield_wrap.c bitfield.so *~

test: bitfield_test; ./bitfield_test
bitfield_test: bitfield.c bitfield_test.c; gcc ${CFLAGS} $^ -o $@

tcl: bitfield.so
bitfield_wrap.c: bitfield.i bitfield.c bitfield.h
	swig -tcl $<

bitfield.so: bitfield.c bitfield_wrap.c;
	gcc $^ -shared $(TCL) $(CFLAGS) -o $@
