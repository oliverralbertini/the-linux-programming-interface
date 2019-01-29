#!/bin/sh
#
# The general idea of this script is to generate a set of shared libraries
# and a main program that can be used to demonstrate the benefits of lazy
# binding.
#
# The script takes two arguments: the number of library source files
# to generate and the number of uniquely named functions that each
# library should contain. The script generates functions with names of
# the form 'func_<libnum>_<funcnum>' in each file. After each library
# source file is created, a shared library is built from the source file.
#
# In addition to creating library source files, this script also generates
# a main program that contains a series of functions that call each of 
# uniquely named functions in all of the shared libraries. However,
# none of these functions are called by the main program, so that, with
# lazy binding, the dynamic linker will never resolve the references to
# the functions symbols. On the other hand, if we prevent lazy binding
# (via the environment variable setting LD_BIND_NOW=1), then all function
# symbols will be resolved, even though they are not needed. With a
# sufficiently large number of shared libraries, there should be a clear
# impact on program execution time, depending on whether lazy binding
# is allowed or not.
#

if test $# != 2; then
    echo 1>&2 "Usage: $0 num-libs num-funcs"
    exit 1
fi

num_libs=$1
num_funcs=$2

MAIN=lb_main
MAIN_SRC=${MAIN}.c
LIB_PREFIX=lb_lib

rm -f ${LIB_PREFIX}_* $MAIN_SRC $MAIN

lib_list=""

#
# Write the header section of the main program
#
cat << EOF > $MAIN_SRC
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

EOF

#
# Generate the library source files and compile each file to
# create a shared library
#
lib_num=0
while test $lib_num -lt $num_libs; do
    lib_num=$(expr $lib_num + 1)
    func_num=0
    while test $func_num -lt $num_funcs; do
        func_num=$(expr $func_num + 1)

        fsuffix="${lib_num}_${func_num}"


cat << EOF >> ${LIB_PREFIX}_${lib_num}.c
int func_$fsuffix(int j)
{
    return j + 1;
}

EOF

cat << EOF >> $MAIN_SRC
extern int func_$fsuffix(int j);

static void
call_func_$fsuffix(int j)
{
    printf("Calling func_$fsuffix returns %d\n", func_$fsuffix(j));
}

EOF
    done

    lib_list="$lib_list ${LIB_PREFIX}_${lib_num}.so"
    echo cc -O0 -Wall -fPIC -shared -o ${LIB_PREFIX}_${lib_num}.so ${LIB_PREFIX}_${lib_num}.c
    cc -O0 -Wall -fPIC -shared -o ${LIB_PREFIX}_${lib_num}.so ${LIB_PREFIX}_${lib_num}.c
done

cat << EOF >> $MAIN_SRC
int
main(int argc, char *argv[])
{
    exit(EXIT_SUCCESS);
}
EOF

echo cc -o $MAIN $MAIN_SRC $lib_list
time cc -o $MAIN $MAIN_SRC $lib_list

echo "Built 'main'"
