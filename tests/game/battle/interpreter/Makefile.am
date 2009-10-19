TESTS=Interpreter_test

bin_PROGRAMS = Interpreter_test
Interpreter_test_SOURCES = Interpreter_test.cpp
Interpreter_test_LDADD = $(top_srcdir)/src/game/battle/interpreter/libint.la $(top_srcdir)/src/game/common/libgameutils.la $(top_srcdir)/src/game/battle/state/libstate.la

LIBCONNECTION = $(top_srcdir)/src/switchbox/libconnection.la

AM_CPPFLAGS = -I$(top_srcdir)/src/game/battle/interpreter -I$(top_srcdir)/src/switchbox -I$(top_srcdir)/src/game/common/ -I$(top_srcdir)/src/game/battle/state
LDADD = $(LIBCONNECTION) libsbcommon.la
#function_tests_LDADD = $(LIBCONNECTION) libsbcommon.la
