AM_CPPFLAGS = -I.. \
              -I$(top_srcdir)/switchbox/src 
AM_LDFLAGS = @SDL_LIBS@  -lSDL_image -lSDL_draw -lSDL_mixer \
             -lboost_thread

TESTS=CMBEvent_tests CMBQueue_tests 

noinst_PROGRAMS = CMBEvent_tests CMBQueue_tests 

CMBEvent_tests_SOURCES = CMBEvent_tests.cpp

CMBQueue_tests_SOURCES = CMBQueue_tests.cpp
