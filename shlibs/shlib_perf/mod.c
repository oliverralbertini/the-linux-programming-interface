/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* mod.c

*/
#include <stdio.h>

static long long x = 0;
long long mod_y;

void
mod_f()
{
    x++;
    if (x % 100000000 == 0)
        printf("%lld\n", x);
}
