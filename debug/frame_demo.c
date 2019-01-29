/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* frame_demo.c

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void
f5(void)
{
    int x = 5;

    printf("Top \n");
    sleep(5);
}

static void
f4(void)
{
    int x = 4;

    f5();
}

static void
f3(void)
{
    f4();
}

static void
f2(void)
{
    int x = 2;

    f3();
}

static void
f1(void)
{
    int x = 1;

    f2();
}

int
main(int argc, char *argv[])
{
    f1();

    exit(EXIT_SUCCESS);
}
