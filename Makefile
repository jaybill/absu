
CC = gcc
FLAGS = -Wall -g -Ivendor\JDAS209F

LIBS = vendor\JDAS209F\judaslib.a

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
	$(LIBS) \
	-o bin\test.exe


clean:
	del src\*.o
	del test\*.o
	del bin\*.*
	del lib\*.*
	del *.o

testdata:
	copy test\test.bmp bin
	copy vendor\JDAS209F\*.WAV bin