CC = gcc
AR = ar
RANLIB = ranlib
CFLAGS = -Wall -g 
SRC = src

LIB = lib\libabsu.a
OBJS = block.o \
	bitmap.o \
	draw.o \
	loop.o \
	keyboard.o \
	video.o

all: $(LIB) bin\test.exe testdata

lib: $(LIB)

.PHONY: testdata clean lib

%.o: $(SRC)\%.s
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: $(SRC)\%.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIB): $(OBJS)
	$(AR) rc $(LIB) $(OBJS)
	$(RANLIB) $(LIB)

bin\test.exe: $(OBJS)
	$(CC) -Wall -g -Ivendor\JDAS209F test\test.c $(OBJS) \
	vendor\JDAS209F\judaslib.a \
	-o bin\test.exe

clean:
	del src\*.o
	del test\*.o
	del bin\*.*
	del lib\*.*
	del *.o

testdata:
	copy test\mole.bmp bin
	copy vendor\JDAS209F\*.WAV bin