CC = gcc
CFLAGS = -Wall
LDFLAGS= -lncurses
ifeq ($(PREFIX),)
    PREFIX := /usr/local/bin
endif
all: build

copy.o: copy.c copy.h
	$(CC) $(CFLAGS) -c $< -o $@

ls.o: ls.c ls.h
	$(CC) $(CFLAGS) -c $< -o $@

gui.o: newgui.c newgui.h
	$(CC) $(CFLAGS) -c $< -o $@

clipboard.o: clipboard.c clipboard.h
	$(CC) $(CFLAGS) -c $< -o $@

rm.o: rm.c rm.h
	$(CC) $(CFLAGS) -c $< -o $@

build: main.c copy.o ls.o gui.o clipboard.o rm.o
	$(CC) $(CFLAGS) $^ -o AlikCommander $(LDFLAGS)

install:
	cp AlikCommander $(PREFIX)/AlikCommander

uninstall:
	rm -rf $(PREFIX)/AlikCommander

clean:
	rm -f *.o AlikCommander