#noinst_LTLIBRARIES = libsync.la 
#libsync_la_SOURCES = Synchronizer.cpp

bin_PROGRAMS = interpreter
interpreter_SOURCES = Interpreter.cpp
LIBCONNECTION = $(top_srcdir)/src/switchbox/libconnection.la $(top_srcdir)/src/game/common/libgameutils.la

AM_CPPFLAGS = -I$(top_srcdir)/src/switchbox -I$(top_srcdir)/src/game/common/
LDADD = $(LIBCONNECTION)
#noinst_LTLIBRARIES = libsbcommon.la
