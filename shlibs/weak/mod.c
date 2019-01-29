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
#include "mod.h"

#ifndef MODNAME
#define MODNAME "MOD"
#endif

#ifndef NO_MOD_FUNC
#ifdef MOD_WEAK
__attribute__((weak))
#endif
void
func(void)
{
    printf("Called func in " MODNAME "\n");
}
#else
#ifdef MOD_WEAK
#error "NO_MOD_FUNC defined *and* MOD_WEAK defined"
#endif
#endif
