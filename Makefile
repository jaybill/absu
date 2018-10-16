
CC = gcc
FLAGS = -Wall -g

all: test.exe testdata

.PHONY: testdata

test.exe: test\test.c
	$(CC) $(FLAGS) -o bin\test.exe \
	test\test.c \
	src\block.c \
	src\bitmap.c \
	src\draw.c \
	src\input.c \
	src\loop.c \
	src\video.c 

clean:
	del src\*.o
	del test\*.o
	del bin\*.*
	del lib\*.*

testdata:
	copy test\test.bmp bin