GCC = gcc
FLAGS = -std=c2x -Wall -Wextra -pedantic

all: main.o file_functions.o
	${GCC} ${FLAGS} $^ -o main

main.o: main.c
	${GCC} ${FLAGS} -c $< -o $@

file_functions.o: file_functions.c
	${GCC} ${FLAGS} -c $< -o $@

clear:
	del *.o
	del *.exe

clear_linux:
	rm *.o
	rm main
