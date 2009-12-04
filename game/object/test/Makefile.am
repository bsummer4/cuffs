AM_CPPFLAGS = -I.. \
              -I$(top_srcdir)/switchbox/src 
AM_LDFLAGS = @SDL_LIBS@  -lSDL_image -lSDL_draw -lSDL_mixer \
             -lboost_thread

TESTS=CMBEvent_tests CMBQueue_tests 

LIBSYNC = $(top_srcdir)/src/game/battle/synchronizer/libsync.la
LIBINT = $(top_srcdir)/src/game/battle/interpreter/libint.la

noinst_PROGRAMS = CMBEvent_tests CMBQueue_tests 

SimpleSynchronizer_test_SOURCES = SimpleSynchronizer_test.cpp
SimpleSynchronizer_test_LDADD = $(LIBSYNC) $(LIBINT) -lboost_thread 

CMBSynchronizer_Test_SOURCES = CMBSynchronizer_Test.cpp
CMBSynchronizer_Test_LDADD = $(LIBSYNC) $(LIBINT) -lboost_thread 

CMBEvent_tests_SOURCES = CMBEvent_tests.cpp

CMBQueue_tests_SOURCES = CMBQueue_tests.cpp

cmb_timestamp_tests_SOURCES = cmb_timestamp_tests.cpp
cmb_timestamp_tests_LDADD = $(LIBSYNC)
