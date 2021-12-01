all: CSORT.o
	gcc -o all CSORT.o

DET.o: CSORT.c CSORT.h
	gcc -c CSORT.c