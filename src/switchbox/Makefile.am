AM_LDFLAGS = -lpthread
TESTS = switchbox_test
check_PROAGRAMS = switchbox_test
bin_PROGRAMS = switchbox
switchbox_SOURCES = switchbox.c message.c switchbox_client.c
python_junk =  *.pyc switchbox_client.py

SOURCES = $(switchbox_SOURCES)
extra_clean: clean; rm -f *wrap.o *wrap.c *.so *.cflow $(python_junk)
ide: call_graph.cflow reverse_call_graph.cflow
TAGS: etags $(SOURCES)
bindings: _switchbox_client.so
check-syntax:; $(CC) -Wall -Wextra -pedantic -fsyntax-only $(CHK_SOURCES)

switchbox_test: message.o test.o; $(CC) $(AM_LDFLAGS) $^ -o $@

switchbox_client_wrap.c: switchbox_client.i
	swig -python -lcpointer.i switchbox_client.i

_switchbox_client.so: message.c switchbox_client.c switchbox_client_wrap.c
	gcc --std=gnu99 -c $^ -I/usr/include/python2.6
	ld $(^:.c=.o) -shared -o _switchbox_client.so

call_graph.cflow: $(SOURCES);cflow $^ > $@
reverse_call_graph.cflow: $(SOURCES);cflow  --reverse $^ > $@
