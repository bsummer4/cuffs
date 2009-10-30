AM_LDFLAGS = -lpthread
AM_CFLAGS = -Wall

bin_PROGRAMS = switchbox switchbox_test chat_test

switchbox_SOURCES = switchbox.c
switchbox_LDADD = libmessaging.la

switchbox_test_SOURCES = test.c
switchbox_test_LDADD = libmessaging.la

chat_test_SOURCES = chat_test.cpp
chat_test_LDADD = libconnection.la

noinst_LTLIBRARIES = libmessaging.la
libmessaging_la_SOURCES = message.c switchbox_client.c

lib_LTLIBRARIES = libconnection.la
libconnection_la_SOURCES = Connection.cpp SwitchboxAdmin.cpp
libconnection_la_LIBADD = libmessaging.la

## SWIG Stuff Autotoolized http://realmike.org/python/swig_linux.htm
BUILT_SOURCES = switchbox_client_wrap.c
SWIG_SOURCES = switchbox_client.i

pkgpyexec_LTLIBRARIES = _switchbox_client.la

pkgpython_PYTHON = switchbox_client.py
SWIG_PYTHON_OPT = -python -lcpointer.i

_switchbox_client_la_SOURCES = message.c switchbox_client.c switchbox_client_wrap.c
_switchbox_client_la_CPPFLAGS = $(SWIG_PYTHON_CPPFLAGS) -I/usr/include/python@PYTHON_VERSION@
_switchbox_client_la_LDFLAGS = -module

switchbox_client_wrap.c :$(SWIG_SOURCES)
	$(SWIG) $(SWIG_PYTHON_OPT) -o $@ $<

#####################
# Manual make rules #
#####################
all-local: _switchbox_client.so

_switchbox_client.so: _switchbox_client.la
	ln -s -f .libs/_switchbox_client.so _switchbox_client.so

SOURCES = $(switchbox_SOURCES)
python_junk =  *.pyc switchbox_client.py
clean-local:; rm -f *wrap.o *wrap.c *.so *.cflow $(python_junk)

# Tools
ide: call_graph.cflow reverse_call_graph.cflow
call_graph.cflow: $(SOURCES);cflow $^ > $@
reverse_call_graph.cflow: $(SOURCES);cflow  --reverse $^ > $@
check-syntax:; $(CC) -Wall -Wextra -pedantic -fsyntax-only $(CHK_SOURCES)
