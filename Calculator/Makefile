CC = gcc
CFLAGS = -Wall
ifeq ($(PREFIX),)
    PREFIX := /usr/local/bin
endif
priorities.o: priorities.c priorities.h
	$(CC) $(CFLAGS) -c $< -o $@

calc.o: calc.c calc.h
	$(CC) $(CFLGS) -c $< -o $@

build: main.c calc.o priorities.o
	$(CC) $(CFLAGS) $^ -o AlikCalc

install: build
	cp AlikCalc $(PREFIX)/AlikCalc

uninstall:
	rm -rf $(PREFIX)/AlikCalc

clean:
	rm -f calc.o priorities.o AlikCalc