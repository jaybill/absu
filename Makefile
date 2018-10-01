# Makefile for DJGPP 2.0

CC = gcc
#FLAGS = -Wall -Os

all: molewarp.exe

molewarp.exe: main.c
	$(CC) $(FLAGS) -o molewarp.exe main.c graphics.c -lgrx20 

clean:
	del *.exe
