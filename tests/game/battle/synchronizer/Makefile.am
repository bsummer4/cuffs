AM_CPPFLAGS = -I$(top_srcdir)/src/game/battle/synchronizer \
              -I$(top_srcdir)/src/game/battle/interpreter \
              -I$(top_srcdir)/src/game/battle/generator \
              -I$(top_srcdir)/src/game/battle/state \
              -I$(top_srcdir)/src/switchbox \
              -I$(top_srcdir)/src/game/common

TESTS=cmb_timestamp_tests CMBEvent_tests CMBQueue_tests synctests.sh 

LIBSYNC = $(top_srcdir)/src/game/battle/synchronizer/libsync.la
LIBINT = $(top_srcdir)/src/game/battle/interpreter/libint.la

noinst_PROGRAMS = CMBSynchronizer_Test CMBEvent_tests CMBQueue_tests SimpleSynchronizer_test cmb_timestamp_tests

SimpleSynchronizer_test_SOURCES = SimpleSynchronizer_test.cpp
SimpleSynchronizer_test_LDADD = $(LIBSYNC) $(LIBINT) -lboost_thread 

CMBSynchronizer_Test_SOURCES = CMBSynchronizer_Test.cpp
CMBSynchronizer_Test_LDADD = $(LIBSYNC) $(LIBINT) -lboost_thread 

CMBEvent_tests_SOURCES = CMBEvent_tests.cpp
CMBEvent_tests_LDADD = $(LIBSYNC) -lboost_thread 

CMBQueue_tests_SOURCES = CMBQueue_tests.cpp
CMBQueue_tests_LDADD = $(LIBSYNC) -lboost_thread 

cmb_timestamp_tests_SOURCES = cmb_timestamp_tests.cpp
cmb_timestamp_tests_LDADD = $(LIBSYNC)
