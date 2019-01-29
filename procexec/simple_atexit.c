/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* simple_atexit.c

   The atexit(3) function can be used to register one or more functions to
   be called at normal process exit. (These routines are not called if the
   process does an _exit(2) or if it is terminated by a signal)

   If more than one exit routine is registered, they are called
   in reverse order of registration.

   Usage: t_atexit [sleep-time]
                   Def = 0 secs
*/
#include "tlpi_hdr.h"

static void exitFunc1(void) {
    printf("Exit function 1 called\n");
}

static void exitFunc2(void) {
    printf("Exit function 2 called\n");
}

int main(int argc, char *argv[]) {
    if (atexit(exitFunc1) != 0)
        fatal("atexit 1");
    if (atexit(exitFunc2) != 0)
        fatal("atexit 2");

    printf("main() about to exit\n");
    exit(EXIT_SUCCESS);
}
