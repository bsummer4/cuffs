# TODO Use automake
# TODO Remove Dependency on file in trunk

CXX = g++
CC = gcc
LIBS = ../object
SBDIR = ../../../trunk/src/switchbox
SBLIBS = $(SBDIR)/.libs
LDFLAGS += $(shell sdl-config --libs)  -lSDL_image -lSDL_draw -lSDL_mixer \
           -lboost_thread $(SBLIBS)/message.o $(SBLIBS)/switchbox_client.o \
           $(SBLIBS)/Connection.o
CFLAGS += -I$(LIBS) -I$(SBDIR) $(shell sdl-config --cflags)
CXXFLAGS = $(CFLAGS) -g -O0
PROGS = dispatcher sync-cat delayed-cat dispatcher2 interpret cat render \
        switchbox-cat game keylisten sleep-cat map-pgm sixty-nine \
        switchbox-connect physics

all: $(PROGS)
clean:; rm -f $(PROGS) *.o
%: %.o; $(CXX) $(LDFLAGS) $^ -o $@
%.o: %.c++; $(CXX) $(CXXFLAGS) -c $^
lib.o: $(LIBS)/lib.c; $(CXX) $(CXXFLAGS) -c $^
dispatcher: lib.o dispatcher.o; $(CXX) $(LDFLAGS) $^ -o $@
dispatcher2: lib.o dispatcher2.o; $(CXX) $(LDFLAGS) $^ -o $@
