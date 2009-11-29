AM_CFLAGS=-I$(top_srcdir)/src/switchbox
AM_CXXFLAGS=$(AM_CFLAGS)

noinst_PROGRAMS = sb_io repeater


LIBCONNECTION=$(top_srcdir)/src/switchbox/libconnection.la
LIBTHREADED=$(top_srcdir)/src/game/common/libthreaded.la

CMSRC=ConnectionManager.cpp


sb_io_SOURCES = sb_io.cpp $(CMSRC)
sb_io_LDADD = $(LIBCONNECTION) $(LIBTHREADED)

repeater_SOURCES = repeater.cpp
repeater_LDADD = $(LIBCONNECTION) $(LIBTHREADED)
