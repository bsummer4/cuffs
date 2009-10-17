AM_CPPFLAGS = -I$(top_srcdir)/src/game/battle/synchronizer -I$(top_srcdir)/src/game/battle/interpreter -I$(top_srcdir)/src/game/battle/generator -I$(top_srcdir)/src/switchbox -I$(top_srcdir)/src/game/common

TESTS=sync_tests

LIBSYNC = $(top_srcdir)/src/game/battle/synchronizer/libsync.la

bin_PROGRAMS = sync_tests

sync_tests_SOURCES = sync_tests.cpp
sync_tests_LDADD = $(LIBSYNC) -lboost_thread
