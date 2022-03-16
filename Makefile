.POSIX:

ifndef DESTDIR
DESTDIR = ~/.local
endif

#---------------------------------------------

all: modules build

#----------------------------------------------

tinyexpr:
	mkdir -p bin
	gcc   -c ./source/modules/Methods/tinyexpr/main.c             -o bin/tinyexpr.o

methods:
	g++   -c ./source/modules/Methods/Methods.cpp                 -o bin/methods.o

arguments:
	g++   -c ./source/modules/ArgumentsParser/ArgumentsParser.cpp -o bin/arguments.o

url:
	g++   -c ./source/modules/URLDownloader/URLDownloader.cpp     -o bin/url.o

assistant:
	g++   -c ./source/modules/AssistantFun/AssistantFun.cpp       -o bin/assistant.o

modules: tinyexpr methods arguments url assistant

#-----------------------------------------------

install: modules build
	mkdir -p ${DESTDIR}/bin
	cp    -f ./bin/iris ${DESTDIR}/bin/iris

clean:
	rm    -r bin

uninstall:
	rm    -f ${DESTDIR}/bin/iris

build:
	g++   -c ./source/main.cpp                                    -o ./bin/iris.o
	g++      ./bin/*.o                                            -o ./bin/iris   -O3 -lpthread -llexbor -lcurl -std=c++20

.PHONY: all build install clean uninstall
