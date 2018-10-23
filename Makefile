
CC = gcc
FLAGS = -Wall -g -Ivendor\sb05_dj2
LDFLAGS = -Lvendor\sb05_dj2 -lsb

all: test.exe testdata

.PHONY: testdata

test.exe: test\test.c
	$(CC) $(FLAGS) -c \
	test\test.c \
	src\block.c \
	src\bitmap.c \
	src\draw.c \
	src\loop.c \
	src\keyboard.s \
	src\video.c 

	$(CC) $(FLAGS) test.o \
	block.o \
	bitmap.o \
	draw.o \
	loop.o \
	keyboard.o \
	video.o \
	-o bin\test.exe $(LDFLAGS)


clean:
	del src\*.o
	del test\*.o
	del bin\*.*
	del lib\*.*
	del *.o

testdata:
	copy test\test.bmp bin