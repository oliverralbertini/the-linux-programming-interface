/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* mtest.c

   A program that just calls malloc() and free(). Used to test preloading.
*/
#include <stdlib.h>
#include <stdio.h>

int
main(int argc, char *argv[])
{
    void * p;

    p = malloc(128);

    printf("\nIn main: malloc() returned %p\n\n", p);

    free(p);

    exit(EXIT_SUCCESS);
}
