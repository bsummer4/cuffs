DESTDIR = /usr/local
CFLAGS = -O3 -W -Wall
VERSION = 1.0

CFLAGS += -std=gnu99
LDFLAGS += -lpthread
HEADERS = message.h switchbox_client.h
SRC = message.c switchbox.c switchbox_client.c
OBJ = $(SRC:.c=.o)
DISTNAME = switchbox-$(VERSION)

all: switchbox
clean:; rm -f $(OBJ) switchbox
switchbox: $(OBJ); $(CC) $(LDFLAGS) $^ -o $@

dist: clean
	mkdir -p $(DISTNAME)
	cp Makefile README $(SRC) $(HEADERS) $(DISTNAME)
	tar cfz $(DISTNAME).tgz $(DISTNAME)
	rm -rf $(DISTNAME)

install: switchbox
	mkdir -p $(DESTDIR)/bin
	mkdir -p $(DESTDIR)/share/doc/switchbox
	install -d $(DESTDIR)/bin $(DESTDIR)/share/doc/switchbox
	install -m 775 switchbox $(DESTDIR)/bin
	install -m 644 README $(DESTDIR)/share/doc/switchbox
