DESTDIR ?= /usr/local
LDLIBS += -lm

RM ?= rm -f


all: batper

README: README.7
	mandoc -Tascii README.7 | col -b > README

clean:
	${RM} batper

install: batper
	install -m755 batper ${DESTDIR}/bin/
	install -m644 batper.1 ${DESTDIR}/man/man1/

uninstall:
	${RM} ${DESTDIR/bin/batper
