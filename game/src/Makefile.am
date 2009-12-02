#AM_LDFLAGS = -lpthread -lboost_thread
AM_LDFLAGS = @SDL_LIBS@  -lSDL_image -lSDL_draw -lSDL_mixer \
           -lboost_thread 
AM_CXXFLAGS = -Wall -O0 -g -I../object -I../../switchbox/src
AM_CFLAGS = $(AM_CXXFLAGS)
#CFLAGS=$(AM_CFLAGS)
#CXXFLAGS=$(AM_CXXFLAGS)


TESTS=../utils/run-test

bin_PROGRAMS = sync-cat delayed-cat interpret cat render switchbox-cat \
               game keylisten sleep-cat map-pgm sixty-nine \
               switchbox-connect physics #dispatcher2 dispatcher

cat_SOURCES = cat.cpp
delayed_cat_SOURCES = delayed-cat.cpp
dispatcher2_SOURCES = dispatcher2.cpp
dispatcher_SOURCES = dispatcher.cpp
game_SOURCES = game.cpp
interpret_SOURCES = interpret.cpp
keylisten_SOURCES = keylisten.cpp
map_pgm_SOURCES = map-pgm.cpp
physics_SOURCES = physics.cpp
render_SOURCES = render.cpp
sleep_cat_SOURCES = sleep-cat.cpp
switchbox_cat_SOURCES = switchbox-cat.cpp
switchbox_connect_SOURCES = switchbox-connect.cpp
sync_cat_SOURCES = sync-cat.cpp


#%: %.o; $(CXX) $(LDFLAGS) $^ -o $@
#.o.cpp: $(CXX) $(AM_CXXFLAGS) -c $^
#lib.o: ../object/lib.c; $(CXX) $(CXXFLAGS) -c $^
#dispatcher: lib.o dispatcher.o; $(CXX) $(AM_CFLAGS) $(AM_LDFLAGS) $^ -o $@
#dispatcher2: lib.o dispatcher2.o; $(CXX) $(AM_CFLAGS) $(AM_LDFLAGS) $^ -o $@
