AM_CFLAGS=-I$(top_srcdir)/src/switchbox
AM_CXXFLAGS=$(AM_CFLAGS)

noinst_PROGRAMS = recorder playback

LIBCONNECTION=$(top_srcdir)/src/switchbox/libconnection.la
LIBTHREADED=$(top_srcdir)/src/game/common/libthreaded.la

recorder_SOURCES = recorder.cpp ConnectionManager.cpp
recorder_LDADD = $(LIBCONNECTION) $(LIBTHREADED)

playback_SOURCES = playback.cpp  ConnectionManager.cpp
playback_LDADD = $(LIBCONNECTION) $(LIBTHREADED)
