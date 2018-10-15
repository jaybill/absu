
CC = gcc
FLAGS = -Wall -O2

all: test.exe

test.exe: test\test.c
	$(CC) $(FLAGS) -o bin\test.exe \
	test\test.c \
	src\block.c \
	src\draw.c \
	src\input.c \
	src\loop.c \
	src\video.c 

clean:
	del src\*.o
	del test\*.o
	del bin\*.*
	del lib\*.*