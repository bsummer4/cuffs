AM_LDFLAGS = -lpthread
TESTS = switchbox_test
check_PROAGRAMS = switchbox_test
bin_PROGRAMS = switchbox
switchbox_SOURCES = switchbox.c message.c
switchbox_test_SOURCES =  message.c test.c

switchbox_test: message.o test.o; $(CC) $(AM_LDFLAGS) $^ -o $@
