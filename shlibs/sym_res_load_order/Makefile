#!/bin/sh

CFLAGS = -Wall

SHLIBS = libx2.so libx1.so liby2.so liby1.so libz3.so libz2.so libz1.so

all: ${SHLIBS} main

clean:
	rm -f ${SHLIBS} main *.o

libx2.so: libx2.o
	cc -o libx2.so -shared -fPIC libx2.c
libx1.so: libx1.o libx2.so
	cc -o libx1.so -shared -fPIC libx1.c libx2.so
liby2.so: liby2.o
	cc -o liby2.so -shared -fPIC liby2.c
liby1.so: liby1.o liby2.so
	cc -o liby1.so -shared -fPIC liby1.c liby2.so
libz3.so: libz3.o
	cc -o libz3.so -shared -fPIC libz3.c
libz2.so: libz2.o libz3.so
	cc -o libz2.so -shared -fPIC libz2.c libz3.so
libz1.so: libz1.o libz2.so
	cc -o libz1.so -shared -fPIC libz1.c libz2.so

main: main.o libx1.so liby1.so libz1.so
	cc -o main main.o -Wl,-rpath-link,. -L. libx1.so liby1.so libz1.so
