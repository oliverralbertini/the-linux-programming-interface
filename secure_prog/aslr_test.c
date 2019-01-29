/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* aslr_test.c

   Run repeatedly to see effect of ASLR

   To see ASLR for the text (main()), compile with: cc -pie -fPIE
*/
#define _GNU_SOURCE
#include <sys/mman.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    int x;
    char *p, *p2;

    p = malloc(16);
    p2 = mmap(NULL, 16384, PROT_READ | PROT_WRITE,
              MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (p2 == MAP_FAILED)
        errExit("mmap");

    printf("main() at:  %p\n", main);
    printf("stack near: %p\n", (void *) &x);
    printf("heap near:  %p\n", p);
    printf("mmap() gave %p\n", p2);

    exit(EXIT_SUCCESS);
}
