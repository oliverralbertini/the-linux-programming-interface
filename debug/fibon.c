/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* fibon.c

   A demo program for use with the debugger; it finds Fibonacci
   number N recursively. (This is not an efficient way of calculating
   Fibonacci numbers, but it it does produce a nice stack of recursive
   stack frames for inspection by the debugger.)

   Usage: fibon N
*/

#include <stdio.h>
#include <stdlib.h>

static long ncalls = 0;

static int
fibon(int n)
{
    int f1, f2, sum;

    ncalls++;

    if (n <= 0)
        return 0;

    if (n == 1)
        return 1;

    f1 = fibon(n - 2);
    f2 = fibon(n - 1);

    sum = f1 + f2;

    return sum;
}

int
main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s num\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("Fibonacci number %d = %d\n", atoi(argv[1]),
            fibon(atoi(argv[1]) - 1));
    printf("ncalls = %ld\n", ncalls);
    exit(EXIT_SUCCESS);
}
