TARGETS = rtld_global_test libfirst.so libsecond.so

all: ${TARGETS}

rtld_global_test: rtld_global_test.c
	cc -g -o rtld_global_test rtld_global_test.c -ldl

libfirst.so: libfirst.c
	cc -g -shared -fPIC -o libfirst.so libfirst.c

libsecond.so: libsecond.c
	cc -g -Wl,-Bsymbolic -shared -fPIC -o libsecond.so libsecond.c

clean:
	${RM} ${TARGETS} *.o


