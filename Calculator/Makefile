CC = gcc
CFLAGS = -Wall
ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif
priorities.o: priorities.c priorities.h
	$(CC) $(CFLAGS) -c $< -o $@

calc.o: calc.c calc.h
	$(CC) $(CFLGS) -c $< -o $@

build: main.c calc.o priorities.o
	$(CC) $(CFLAGS) $^ -o AlikCalc

install: build
	# ???

clean:
	rm calc.o priorities.o AlikCalc