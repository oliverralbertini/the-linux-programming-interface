#!/bin/sh

# Discover the pathname of the dynamic linker on this system by
# using ldd(1) to inspect a standard executable

interp="$(ldd /bin/ls | awk '$1 ~ /ld-linux/ {print  $1}')"

# Build the shared library, explicitly specifying an entry point function
# and also the name of the interpreter that should be placed in the
# ".interp" section of the shared library

cc -DINTERP="\"$interp\"" -fPIC -shared libtest.c -o libtest.so \
		-Wl,-soname,libtest.so \
		-Wl,--entry,entry_func

echo
echo "Executing a program that uses a function in the shared library:"
cc -o prog prog.c ./libtest.so
LD_LIBRARY_PATH=. ./prog

echo
echo "Executing the shared library:"
./libtest.so
