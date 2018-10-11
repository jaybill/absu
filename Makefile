
BIN       	= test
SRCDIR    	= src
BINDIR 		= bin
LIB			= libabsu.a
LIBDIR		= lib
TESTDIR		= test

CC        	= $(DJGPP_CC)
CFLAGS    	= -DHAVE_STDBOOL_H=1 -w 
LDFLAGS   	= -L${LIBDIR} -labsu
AR			= ar

# All source files (*.c) and their corresponding object files.
SRC       = $(shell find $(SRCDIR) -name "*.c" 2> /dev/null)
OBJS      = $(SRC:%.c=%.o)

.PHONY: clean dir lib
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
	${CC} -c -w ${TESTDIR}/${BIN}.c -o ${TESTDIR}/${BIN}.o
	${CC} ${TESTDIR}/test.o -o ${BINDIR}/${BIN}.exe ${LDFLAGS}

${LIBDIR}/${LIB}: ${OBJS}
	${AR} rcs ${LIBDIR}/${LIB} $+

lib: dir ${LIBDIR}/${LIB}

all: dir ${LIBDIR}/${LIB} ${BINDIR}/${BIN}.exe

clean:
	rm -rf ${LIBDIR}/*
	rm -rf ${BINDIR}/*
	rm -f ${OBJS}
