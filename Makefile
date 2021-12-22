.POSIX:

ifndef DESTDIR
DESTDIR = ~/.local
endif

# C++20
#OPTS = -fmodules-ts -std=c++20
#STANDARD_MODULES = iostream string vector algorithm
#MODULES = source/modules

#---------------------------------------------

all: build

#----------------------------------------------

install: build
	mkdir -p ${DESTDIR}/bin
	cp -f ./bin/iris ${DESTDIR}/bin/iris

clean:
	rm -r bin

uninstall:
	rm -f ${DESTDIR}/bin/iris

build:
	mkdir -p bin
	gcc source/iris.cpp -o bin/iris
#	gcc -c source/iris.cpp -o bin/iris.o
#	g++ ${OPTS} bin/iris.o -o bin/iris

#-----------------c++20-------------------------

#modules:
#	mkdir -p bin
#	gcc ${OPTS} -xc++-system-header ${STANDARD_MODULES}
#	gcc -c ${OPTS} ${MODULES}/ArgumentParser/arguments.cpp -o bin/arguments.o
#	gcc -c ${OPTS} ${MODULES}/Util/util.cpp -o bin/util.o
#	gcc -c ${MODULES}/tinyexpr/tinyexpr.c -o bin/tinyexpr.o



.PHONY: all build install clean uninstall
