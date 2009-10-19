AM_CPPFLAGS = -I$(top_srcdir)/src/game/battle/synchronizer -I$(top_srcdir)/src/game/battle/interpreter -I$(top_srcdir)/src/game/battle/generator -I$(top_srcdir)/src/switchbox -I$(top_srcdir)/src/game/common

TESTS=CMBEvent_tests CMBQueue_tests synctests.sh

LIBSYNC = $(top_srcdir)/src/game/battle/synchronizer/libsync.la
LIBINT = $(top_srcdir)/src/game/battle/interpreter/libint.la

noinst_PROGRAMS = sync_tests CMBEvent_tests CMBQueue_tests SimpleSynchronizer_test

SimpleSynchronizer_test_SOURCES = SimpleSynchronizer_test.cpp
SimpleSynchronizer_test_LDADD = $(LIBSYNC) $(LIBINT) -lboost_thread 

sync_tests_SOURCES = sync_tests.cpp
sync_tests_LDADD = $(LIBSYNC) $(LIBINT) -lboost_thread 

CMBEvent_tests_SOURCES = CMBEvent_tests.cpp
CMBEvent_tests_LDADD = $(LIBSYNC) -lboost_thread 

CMBQueue_tests_SOURCES = CMBQueue_tests.cpp
CMBQueue_tests_LDADD = $(LIBSYNC) -lboost_thread 
