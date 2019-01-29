/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* main.c

*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void
x(void)
{
    printf("Called %s %s()\n", __FILE__, __func__);
}

int
main(int argc, char *argv[])
{
    extern void mod1_a();
    extern void mod1_b();
    extern void mod2_m();

    printf("%s::%s(): about to call x()\n", __FILE__, __func__);
    x();

    mod1_a();
    printf("\n");
    mod1_b();
    printf("\n");
    mod2_m();

    exit(EXIT_SUCCESS);
}
