
BIN       	= test
SRCDIR    	= src
BINDIR 		= bin
LIB			= libabsu.a
LIBDIR		= lib
TESTDIR		= test

CC        	= $(DJGPP_CC)
CFLAGS    	= -g -DHAVE_STDBOOL_H=1 -w
LDFLAGS   	= -L${LIBDIR} -labsu
AR			= ar

# All source files (*.c) and their corresponding object files.
SRC       = $(shell find $(SRCDIR) -name "*.c" 2> /dev/null)
OBJS      = $(SRC:%.c=%.o)

.PHONY: clean dir lib testdata
default: all

check_djgpp:
	@if [ -z "$$DJGPP_CC" ]; then \
        echo "To compile, you'll need to set the DJGPP_CC environment variable to a DJGPP GCC binary, e.g. /usr/local/djgpp/bin/i586-pc-msdosdjgpp-gcc"; \
        exit 2; \
	fi

dir:
	@mkdir -p ${BINDIR}
	@mkdir -p ${LIBDIR}

%.o: %.c | check_djgpp
	${CC} -c -o $@ $? ${CFLAGS}

${BINDIR}/${BIN}.exe: | ${LIBDIR}/${LIB}
	${CC} -c -g -w ${TESTDIR}/${BIN}.c -o ${TESTDIR}/${BIN}.o
	${CC} ${TESTDIR}/test.o -g -o ${BINDIR}/${BIN}.exe ${LDFLAGS}
	${CC} ${TESTDIR}/bitmap.c -o ${BINDIR}/BITMAP.EXE -g -w

${LIBDIR}/${LIB}: ${OBJS}
	${AR} rcs ${LIBDIR}/${LIB} $+

lib: dir ${LIBDIR}/${LIB}

all: dir ${LIBDIR}/${LIB} ${BINDIR}/${BIN}.exe testdata

testdata:
	cp ${TESTDIR}/test.bmp ${BINDIR}
	cp ${TESTDIR}/rocket.bmp ${BINDIR}

clean:
	rm -rf ${LIBDIR}/*
	rm -rf ${BINDIR}/*
	rm -f ${OBJS}
