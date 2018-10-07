# Makefile for DJGPP 2.0

CC = gcc
FLAGS = -Wall -Os

all: molewarp.exe

molewarp.exe: main.c
	redir -o gcc.log -eo $(CC) $(FLAGS) -o molewarp.exe \
	main.c \
	video.c

clean:
	del *.exe