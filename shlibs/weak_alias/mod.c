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

static void
defaultFunc(char *id)
{
    printf("%s: Called defaultFunc() in " __FILE__ "\n", id);
}

void x1(char *id) __attribute__((weak, alias ("defaultFunc")));
void x2(char *id) __attribute__((weak, alias ("defaultFunc")));
void x3(char *id) __attribute__((weak, alias ("defaultFunc")));

void
xyz(void)
{
    x1("x1");
    x2("x2");
    x3("x3");
}
