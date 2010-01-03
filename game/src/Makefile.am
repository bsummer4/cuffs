AM_LDFLAGS = @SDL_LIBS@ -lSDL_image -lSDL_gfx -lSDL_mixer -lboost_thread
AM_CXXFLAGS = -Wall -O0 -g -I../object @SDL_CFLAGS@
AM_CFLAGS = $(AM_CXXFLAGS)

bin_PROGRAMS = sync-cat delayed-cat interpret cat render game keylisten \
               sleep-cat map-pgm physics dispatcher2 dispatcher keystate \
               paint_window physics

cat_SOURCES = cat.cpp
paint_window_SOURCES = paint_window.cpp
delayed_cat_SOURCES = delayed-cat.cpp
dispatcher2_SOURCES = dispatcher2.cpp ../object/lib.c
dispatcher_SOURCES = dispatcher.cpp ../object/lib.c
game_SOURCES = game.cpp
interpret_SOURCES = interpret.cpp
keylisten_SOURCES = keylisten.cpp
keystate_SOURCES = keystate.cpp
map_pgm_SOURCES = map-pgm.cpp
physics_SOURCES = physics.cpp
render_SOURCES = render.cpp
sleep_cat_SOURCES = sleep-cat.cpp
sync_cat_SOURCES = sync-cat.cpp
