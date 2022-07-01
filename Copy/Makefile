CC = gcc
CFLAGS = -Wall
ifeq ($(PREFIX),)
    PREFIX := /usr/local/bin
endif
all: build

copy.o: copy.c copy.h
	$(CC) $(CFLAGS) -c $< -o $@

build: main.c copy.o
	$(CC) $(CFLAGS) $^ -o AlikCopy

install:
	cp AlikCopy $(PREFIX)/AlikCopy

uninstall:
	rm -rf $(PREFIX)/AlikCopy

clean:
	rm -f copy.o AlikCopy