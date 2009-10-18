noinst_LTLIBRARIES = libint.la 
libint_la_SOURCES = Interpreter.cpp

bin_PROGRAMS = Interpreter_test
Interpreter_test_SOURCES = Interpreter_test.cpp
Interpreter_test_LDADD = $(top_srcdir)/src/game/common/libgameutils.la libint.la

AM_CPPFLAGS = -I$(top_srcdir)/src/game/common
