CC = gcc
CFLAGS = -Wall -Wextra

all: main

main: src/main.c
	$(CC) $(CFLAGS) -o bin/$@ $< -lc

run:
	bin/main
