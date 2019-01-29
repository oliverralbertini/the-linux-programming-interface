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
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef NO_MOD_H
#include "mod.h"
#endif

#ifdef MAIN_PROVIDES_WEAK_DECL
__attribute__((weak)) void func(void);
#endif

#if MAIN_PROVIDES_WEAK_FUNC
__attribute__((weak))
#endif
#if defined(MAIN_PROVIDES_WEAK_FUNC) || defined(MAIN_PROVIDES_STRONG_FUNC)
void
func(void)
{
    printf("Called func in " __FILE__ "\n");
}
#endif

int
main(int argc, char *argv[])
{
    if (func)
        func();
    else
        printf("No definition exists for func\n");

    exit(EXIT_SUCCESS);
}
