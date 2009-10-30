#bin_PROGRAMS = PhysicsTest
#PhysicsTest_SOURCES = generatorTest.cpp
#PhysicsTest_LDADD = $(top_srcdir)/src/game/battle/generator/libgen.la $(top_srcdir)/src/game/common/ $(top_srcdir)/src/
#
#AM_CPPFLAGS = -I$(top_srcdir)/src/switchbox -I$(top_srcdir)/src/game/battle/Physics 
#LDADD = $(LIBCONNECTION) libsbcommon.la
#TESTS=Interpreter_test

TESTS=Physics_Test
bin_PROGRAMS=Physics_Test
Physics_Test_SOURCES=Physics.cpp
Physics_Test_LDADD=$(top_srcdir)/src/game/battle/state/libstate.la $(top_srcdir)/src/game/common/libgameutils.la
#$(top_srcdir)/src/game/common/libgameutils.la $(top_srcdir)/src/game/battle/state/libstate.la

#LIBCONNECTION=$(top_srcdir)/src/switchbox/libconnection.la

AM_CPPFLAGS=-I$(top_srcdir)/src/game/battle/state -I$(top_srcdir)/src/game/common 
#-I$(top_srcdir)/src/game/common/ -I$(top_srcdir)/src/game/battle/state
#LDADD=$(LIBCONNECTION) libsbcommon.la
#function_tests_LDADD = $(LIBCONNECTION) libsbcommon.la
