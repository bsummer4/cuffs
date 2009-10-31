AM_CPPFLAGS = -I$(top_srcdir)/src/game/battle/synchronizer \
              -I$(top_srcdir)/src/game/battle/interpreter \
              -I$(top_srcdir)/src/game/battle/generator \
              -I$(top_srcdir)/src/game/battle/state \
              -I$(top_srcdir)/src/switchbox \
              -I$(top_srcdir)/src/game/common

TESTS=CMBgo.sh

LIBSYNC = $(top_srcdir)/src/game/battle/synchronizer/libsync.la
LIBINT = $(top_srcdir)/src/game/battle/interpreter/libint.la
LIBGEN = $(top_srcdir)/src/game/battle/generator/libgen.la

noinst_PROGRAMS = CMBclientTest 
CMBclientTest_SOURCES = CMBclientTest.cpp
CMBclientTest_LDADD = $(LIBSYNC) $(LIBINT) $(LIBGEN) -lboost_thread 

noinst_PROGRAMS = CMBsingleTest 
CMBsingleTest_SOURCES = CMBsingleTest.cpp
CMBsingleTest_LDADD = $(LIBSYNC) $(LIBINT) $(LIBGEN) -lboost_thread 
