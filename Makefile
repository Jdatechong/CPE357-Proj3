CC = gcc
CFLAGS = -Wall -ansi -g -pedantic
MAIN = hencode hdecode
all : $(MAIN)

hencode : hencode.c hencode.h
	$(CC) $(CFLAGS) -o hencode hencode.c
hdecode : hdecode.c hencode.h
	$(CC) $(CFLAGS) -o hdecode hdecode.c hencode.h 


clean :
	rm *.o $(MAIN) core

