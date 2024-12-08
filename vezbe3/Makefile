GCC = gcc
FLAGS = -std=c2x -Wall -Wextra -pedantic

all: main.o
	${GCC} ${FLAGS} $^ -o main

main.o: main.c
	${GCC} ${FLAGS} -c $< -o $@

clear:
	del *.o
	del *.exe
