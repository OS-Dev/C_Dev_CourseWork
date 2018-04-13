#
# Student makefile for File Utilities Lab
# 
CC = gcc
CFLAGS = -g -Wall

all: mycopy mypager

mycopy: mycopy.c
	$(CC) $(CFLAGS) -o mycopy mycopy.c 
	
mypager: mypager.c
	$(CC) $(CFLAGS) -o mypager mypager.c 
	
clean:
	rm -f *.o
	rm -f mycopy mypager
	