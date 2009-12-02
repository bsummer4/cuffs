AM_LDFLAGS = -lpthread -lboost_thread
AM_CXXFLAGS = -Wall -O0 -g -I../object -I../../switchbox/src
AM_CFLAGS = $(AM_CXXFLAGS)


TESTS=../utils/run-test

bin_PROGRAMS = sync-cat delayed-cat interpret cat render switchbox-cat \
               game keylisten sleep-cat map-pgm sixty-nine \
               switchbox-connect physics #dispatcher2 dispatcher

#sync_cat_SOURCES = sync-cat.cpp

#cat_SOURCES = cat.cpp


#%: %.o; $(CXX) $(LDFLAGS) $^ -o $@
%.o: %.cpp $(CXX) $(AM_CXXFLAGS) -c $^
#lib.o: ../object/lib.c; $(CXX) $(CXXFLAGS) -c $^
#dispatcher: lib.o dispatcher.o; $(CXX) $(AM_CFLAGS) $(AM_LDFLAGS) $^ -o $@
#dispatcher2: lib.o dispatcher2.o; $(CXX) $(AM_CFLAGS) $(AM_LDFLAGS) $^ -o $@
