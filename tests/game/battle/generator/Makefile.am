#bin_PROGRAMS = generatorTest
#generatorTest_SOURCES = generatorTest.cpp
#generatorTest_LDADD = $(top_srcdir)/src/game/battle/generator/libgen.la $(top_srcdir)/src/game/common/ $(top_srcdir)/src/
#
#AM_CPPFLAGS = -I$(top_srcdir)/src/switchbox -I$(top_srcdir)/src/game/battle/generator 
#LDADD = $(LIBCONNECTION) libsbcommon.la
#TESTS=Interpreter_test

TESTS=Generator_Test
bin_PROGRAMS=Generator_Test
Generator_Test_SOURCES=Generator_Test.cpp
Generator_Test_LDADD=$(top_srcdir)/src/game/battle/generator/libgen.la $(top_srcdir)/src/switchbox/libconnection.la
#$(top_srcdir)/src/game/common/libgameutils.la $(top_srcdir)/src/game/battle/state/libstate.la

#LIBCONNECTION=$(top_srcdir)/src/switchbox/libconnection.la

AM_CPPFLAGS=-I$(top_srcdir)/src/game/battle/generator -I$(top_srcdir)/src/switchbox 
#-I$(top_srcdir)/src/game/common/ -I$(top_srcdir)/src/game/battle/state
#LDADD=$(LIBCONNECTION) libsbcommon.la
#function_tests_LDADD = $(LIBCONNECTION) libsbcommon.la
