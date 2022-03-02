PREFIX = i586-pc-msdosdjgpp-
CC = $(PREFIX)gcc
AR = $(PREFIX)ar
RANLIB = $(PREFIX)ranlib
CFLAGS = -Wall -g 
SRC = src
RM = rm -rf
CP = cp
MKDIR_P = mkdir -p
BIN_DIR = bin
BIN_NAME = bin/test.exe
LIB_DIR = lib
LIB_NAME = lib/libabsu.a
OBJS = block.o \
	bitmap.o \
	draw.o \
	logger.o \
	loop.o \
	keyboard.o \
	video.o 

all: $(LIB_NAME) $(BIN_NAME) testdata

.PHONY: testdata clean $(LIB_NAME) directories

directories:
	$(MKDIR_P) $(BIN_DIR)
	$(MKDIR_P) $(LIB_DIR)
	
%.o: $(SRC)/%.s
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: $(SRC)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIB_NAME): $(OBJS) directories
	$(AR) rc $(LIB_NAME) $(OBJS)
	$(RANLIB) $(LIB_NAME)

$(BIN_NAME): $(OBJS)
	$(CC) -Wall -g -Ivendor/jdas209f test/test.c $(OBJS) \
	vendor/jdas209f/judaslib.a \
	-o $(BIN_NAME)

clean:
	$(RM) *.o
	$(RM) $(BIN_DIR)
	$(RM) $(LIB_DIR)

testdata:
	$(CP) test/test.bmp bin
	$(CP) vendor/jdas209f/*.wav bin