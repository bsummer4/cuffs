TESTS=runtests.py

LIBCONNECTION = ../src/libconnection.la

AM_CPPFLAGS = -I../src
LDADD = $(LIBCONNECTION) libsbcommon.la

noinst_LTLIBRARIES = libsbcommon.la 

libsbcommon_la_SOURCES = SBTestCommon.cpp

bin_PROGRAMS = function_tests edgecase_tests latency_test

function_tests_SOURCES = function_tests.cpp
#function_tests_LDADD = $(LIBCONNECTION) libsbcommon.la

edgecase_tests_SOURCES = edgecase_tests.cpp
#edgecase_tests_LDADD = $(LIBCONNECTION) libsbcommon.la

latency_test_SOURCES = latency_test.cpp
