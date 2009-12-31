# Parameters: These are open to change by the user
DESTDIR = /usr/local
CC = gcc
CFLAGS = -O3 -W -Wall

# Without '-fno-strict-aliasing' gcc bitches about our recasting of
# SBMessage.data to various things.
VERSION = 1.0
CFLAGS += -std=gnu99 -fno-strict-aliasing
LDFLAGS += -lpthread
HEADERS = message.h switchbox_client.h
SRC = message.c switchbox.c switchbox_client.c sixty-nine.c switchbox-cat.c
OBJ = $(SRC:.c=.o)
DISTNAME = switchbox-$(VERSION)
PROGS = sixty-nine switchbox switchbox-cat
SCRIPTS = switchbox.sh

all: $(PROGS)
clean:; rm -f $(OBJ) $(PROGS)
switchbox: message.o switchbox.o switchbox_client.o; $(CC) $(LDFLAGS) $^ -o $@
sixty-nine: sixty-nine.o; $(CC) $^ -o $@
switchbox-cat: message.o switchbox_client.o switchbox-cat.o
	$(CC) $(LDFLAGS) $^ -o $@

dist: clean
	mkdir -p $(DISTNAME)
	cp Makefile README $(SRC) $(HEADERS) $(SCRIPTS) $(DISTNAME)
	tar cfz $(DISTNAME).tgz $(DISTNAME)
	rm -rf $(DISTNAME)

install: $(PROGS)
	mkdir -p $(DESTDIR)/bin
	mkdir -p $(DESTDIR)/share/doc/switchbox
	install -d $(DESTDIR)/bin $(DESTDIR)/share/doc/switchbox
	install -m 644 README $(DESTDIR)/share/doc/switchbox
	install -m 775 -t $(DESTDIR)/bin $(PROGS) $(SCRIPTS)

uninstall:
	cd $(DESTDIR)/bin; rm -f $(PROGS) $(SCRIPTS)
	rm -f $(DESTDIR)/share/doc/switchbox/README
