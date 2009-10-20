noinst_LTLIBRARIES = libgen.la
libgen_la_SOURCES = Generator.cpp

#bin_PROGRAMS = Generator
#Generator_SOURCES = Generator.cpp
AM_CPPFLAGS = -I$(top_srcdir)/src/switchbox

LIBCONNECTION = $(top_srcdir)/src/switchbox/libconnection.la
LDADD = $(LIBCONNECTION)
