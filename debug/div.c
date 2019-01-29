/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* div.c

   Program to demonstrate the use of the debugger.

   Run with two integer arguments, it divides the first by the second.
   If the second argument is zero, then the program will crash with a
   divide-by-zero exception, and the debugger can be used to inspect
   the program from the resulting core dump.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int i,j;

static int k;

static void
do_div(int i, int j)
{
    k = i / j;
    printf("%d\n", k);
}

int
main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <int> <int>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    i = atoi(argv[1]);
    j = atoi(argv[2]);
    do_div(i,j);

    exit(EXIT_SUCCESS);
}
