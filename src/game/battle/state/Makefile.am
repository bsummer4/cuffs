#noinst_LTLIBRARIES = libsync.la 
#libsync_la_SOURCES = Synchronizer.cpp

bin_PROGRAMS = state
interpreter_SOURCES = State.cpp
LIBCONNECTION = $(top_srcdir)/src/switchbox/libconnection.la

AM_CPPFLAGS = -I$(top_srcdir)/src/switchbox
LDADD = $(LIBCONNECTION)
#noinst_LTLIBRARIES = libsbcommon.la
