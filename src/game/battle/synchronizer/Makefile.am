AM_CFLAGS = -I$(top_srcdir)/src/game/common -I$(top_srcdir)/src/switchbox -I$(top_srcdir)/src/game/battle/interpreter
AM_CPPFLAGS = $(AM_CFLAGS)

noinst_LTLIBRARIES = libsync.la 
libsync_la_SOURCES = Synchronizer.cpp SimpleSynchronizer.cpp
libsync_la_LIBADD = $(top_srcdir)/src/game/common/libthreaded.la
