CFLAGS = -g

all: main

main: main.o libmod1.so libmod2.so

libmod1_a.o: libmod1_a.c
	cc -g -c -fPIC libmod1_a.c

libmod1_b.o: libmod1_b.c
	cc -g -c -fPIC libmod1_b.c

libmod2_m.o: libmod2_m.c
	cc -g -c -fPIC libmod2_m.c

libmod1.so: libmod1_a.o libmod1_b.o
	cc -g -shared -o libmod1.so libmod1_a.o libmod1_b.o

libmod2.so: libmod2_m.o
	cc -g -shared -o libmod2.so libmod2_m.o

clean:
	rm *.o *.so main
