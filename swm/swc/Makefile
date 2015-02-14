CFLAGS	+=-I/opt/X11/include -std=c99 -g -pedantic -Wall -Os
LDFLAGS	+=-L/opt/X11/lib -lxcb -lxcb-keysyms
SRC	=  swc.c
OBJ	=  ${SRC:.c=.o}
RM	?= /bin/rm
PREFIX	?= /usr

all: swc

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $<

swc: ${OBJ}
	@echo LD $@
	@${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	${RM} swc ${OBJ}

install:
	install -m 755 swc ${DESTDIR}${PREFIX}/bin/swc

uninstall:
	rm ${DESTDIR}${PREFIX}/bin/swc
