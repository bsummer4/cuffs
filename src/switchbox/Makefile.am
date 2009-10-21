AM_LDFLAGS = -lpthread

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

#####################
# Manual make rules #
#####################

all-local: bindings

SOURCES = $(switchbox_SOURCES)
python_junk =  *.pyc switchbox_client.py
clean-local:; rm -f *wrap.o *wrap.c *.so *.cflow $(python_junk)

# Python Bindings
bindings: _switchbox_client.so

switchbox_client_wrap.c: switchbox_client.i
	swig -python -lcpointer.i switchbox_client.i

_switchbox_client.so: message.c switchbox_client.c switchbox_client_wrap.c
	$(CC) --std=gnu99 -c $^ -I/usr/include/python2.6
	$(CC) $(^:.c=.o) -shared -o _switchbox_client.so

# Tools
ide: call_graph.cflow reverse_call_graph.cflow
call_graph.cflow: $(SOURCES);cflow $^ > $@
reverse_call_graph.cflow: $(SOURCES);cflow  --reverse $^ > $@
check-syntax:; $(CC) -Wall -Wextra -pedantic -fsyntax-only $(CHK_SOURCES)
