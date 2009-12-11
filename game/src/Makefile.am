#AM_LDFLAGS = -lpthread -lboost_thread
AM_LDFLAGS = @SDL_LIBS@  -lSDL_image -lSDL_gfx -lSDL_mixer \
             -lboost_thread
AM_CXXFLAGS = -Wall -O0 -g -I../object -I../../switchbox/src @SDL_CFLAGS@
AM_CFLAGS = $(AM_CXXFLAGS)

bin_PROGRAMS = sync-cat delayed-cat interpret cat render switchbox-cat \
               game keylisten sleep-cat map-pgm sixty-nine \
               switchbox-connect physics dispatcher2 dispatcher repeater \
               sb_io keystate paint_window

cat_SOURCES = cat.cpp
paint_window_SOURCES = paint_window.cpp
delayed_cat_SOURCES = delayed-cat.cpp
dispatcher2_SOURCES = dispatcher2.cpp ../object/lib.c
dispatcher_SOURCES = dispatcher.cpp ../object/lib.c
game_SOURCES = game.cpp
interpret_SOURCES = interpret.cpp
keylisten_SOURCES = keylisten.cpp
keystate_SOURCES = keystate.cpp
repeater_SOURCES = repeater.cpp
repeater_LDADD = ../../switchbox/src/libmessaging.la
map_pgm_SOURCES = map-pgm.cpp
physics_SOURCES = physics.cpp
render_SOURCES = render.cpp
sleep_cat_SOURCES = sleep-cat.cpp
switchbox_cat_SOURCES = switchbox-cat.cpp
switchbox_cat_LDADD = ../../switchbox/src/libmessaging.la
switchbox_connect_SOURCES = switchbox-connect.cpp
switchbox_connect_LDADD = ../../switchbox/src/libmessaging.la
sync_cat_SOURCES = sync-cat.cpp
sb_io_SOURCES = sb_io.cpp
sb_io_LDADD = ../../switchbox/src/libmessaging.la
