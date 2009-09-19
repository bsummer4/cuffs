TESTS=runtests.py

AM_CPPFLAGS = -I$(top_srcdir)/src/switchbox

LIBCONNECTION = $(top_srcdir)/src/switchbox/libconnection.la

bin_PROGRAMS = function_tests

function_tests_SOURCES = function_tests.cc
function_tests_LDADD = $(LIBCONNECTION)
