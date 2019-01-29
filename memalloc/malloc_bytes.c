/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* malloc_bytes.c

   Copyright Michael Kerrisk,  Sep 00

   malloc_bytes [num-bytes]

   Allocate memory until no more can be allocated.

   Can be used to test limits on malloc()-ed memory and also to examine
   where blocks of various size are allocated in the process address space.
*/
#define _BSD_SOURCE
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    char *p;
    int x;
    int numBytes;

    numBytes = (argc > 1) ? getInt(argv[1], 0, "num-bytes") : 100;
    for (;;) {
        p = malloc(numBytes);
        if (p == NULL)
            errExit("malloc");
        x = *(int *) (p-4);
        printf("New block = %p, current break %p\n", p, sbrk(0));
        printf("%d\n", x);
        usleep(10);
    }
    exit(EXIT_SUCCESS);
}
