GCC = gcc
FLAGS = -std=c2x

all: main.o file_work.o
	${GCC} ${FLAGS} $^ -o main

main.o: main.c file_work.o
	${GCC} ${FLAGS} -c $< -o $@

file_work.o: file_work.c
	${GCC} ${FLAGS} -c $< -o $@

clear:
	del data.bin
	del *.o
	del *.exe

clear_linux:
	rm data.bin
	rm *.o
	rm main