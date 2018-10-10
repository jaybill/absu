
CC        = $(DJGPP_CC)
VENDOR    = deps
CFLAGS    = -DHAVE_STDBOOL_H=1 -w
LDFLAGS   = 

BIN       = molewarp.exe
SRCDIR    = src
DISTDIR   = dist

# All source files (*.c) and their corresponding object files.
SRC       = $(shell find $(SRCDIR) -name "*.c" 2> /dev/null)
OBJS      = $(SRC:%.c=%.o)

.PHONY: clean dir
default: all

check_djgpp:
	@if [ -z "$$DJGPP_CC" ]; then \
        echo "To compile, you'll need to set the DJGPP_CC environment variable to a DJGPP GCC binary, e.g. /usr/local/djgpp/bin/i586-pc-msdosdjgpp-gcc"; \
        exit 2; \
	fi

dir:
	@mkdir -p ${DISTDIR}

%.o: %.c | check_djgpp
	${CC} -c -o $@ $? ${CFLAGS}

${DISTDIR}/${BIN}: ${OBJS} | check_djgpp
	${CC} -o ${DISTDIR}/${BIN} $+ ${LDFLAGS}

all: dir ${DISTDIR}/${BIN}

clean:
	rm -rf ${DISTDIR}/*
	rm -f ${OBJS}
