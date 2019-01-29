/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* prime_bug.c

   Calculate all prime numbers up to limit given in argv[1] (def: 100).

   Contains bugs!
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Return the truncated integer square root of 'num', calculated via an
   adaptation of the Newton-Raphson algorithm */

static int
sqroot(int num)
{
    int oldX, X, newX;

    oldX = 0;
    newX = -1;
    X = num/2;

    for (;;) {
        newX = (num / X + X) / 2;
        if ((newX == X) || (newX == oldX))
            break;
        oldX = X;
        X = newX;
    }
    return (newX < X) ? newX : X;
}

static int
test_prime(int test)
{
    int root, fac;

    root = sqroot(test);
    for (fac = 3; fac < root; fac += 2)
        if (test % fac == 0)
            return 0;

    return 1;
}

int
main(int argc, char *argv[])
{
    int lim, test;
    int is_prime;

    lim = (argc > 1) ? atoi(argv[1]) : 100;

    printf("2\n");      /* 2 is the only even prime number */

    /* All other prime numbers are odd; now test for those */

    for (test = 3; test <= lim; test += 1) {
        is_prime = test_prime(test);

        if (is_prime)
            printf("%d\n", test);
    }

    exit(EXIT_SUCCESS);
}
