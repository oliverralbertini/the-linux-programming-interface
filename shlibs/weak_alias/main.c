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
#include <stdlib.h>
#include <unistd.h>

extern void xyz(void);

void
x1(char *id)
{
    printf("%s: called version in " __FILE__ "\n", id);
}

int
main(int argc, char *argv[])
{
    xyz();
    exit(EXIT_SUCCESS);
}
