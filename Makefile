.POSIX:

ifndef DESTDIR
DESTDIR = ~/.local
endif

all: build clean

#-DCURL_STATICLIB
build:
	g++ -std=c++20 -c source/iris.cpp -o iris.o
	cc -c source/libraries/tinyexpr/tinyexpr.c -o source/libraries/tinyexpr/tinyexpr.o
	g++ -std=c++20 iris.o source/libraries/tinyexpr/tinyexpr.o -lcurl -o ./bin/iris

run: build
	./bin/iris

install: build
	mkdir -p ${DESTDIR}/bin
	cp -f ./bin/iris ${DESTDIR}/bin/iris

clean:
	rm iris.o source/libraries/tinyexpr/tinyexpr.o

uninstall:
	rm -f ${DESTDIR}/bin/iris

.PHONY: all install uninstall
