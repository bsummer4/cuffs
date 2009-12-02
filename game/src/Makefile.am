AM_LDFLAGS = -lpthread
AM_CXXFLAGS = -Wall -O0 -g -I../object -I../../switchbox/

bin_PROGRAMS = sync-cat delayed-cat interpret cat render switchbox-cat \
               game keylisten sleep-cat map-pgm sixty-nine \
               switchbox-connect physics #dispatcher2 dispatcher

%: %.o; $(CXX) $(LDFLAGS) $^ -o $@
%.o: %.c++; $(CXX) $(AM_CXXFLAGS) -c $^
#lib.o: ../object/lib.c; $(CXX) $(CXXFLAGS) -c $^
#dispatcher: lib.o dispatcher.o; $(CXX) $(AM_CFLAGS) $(AM_LDFLAGS) $^ -o $@
#dispatcher2: lib.o dispatcher2.o; $(CXX) $(AM_CFLAGS) $(AM_LDFLAGS) $^ -o $@
