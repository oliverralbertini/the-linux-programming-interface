/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* simple_longjmp.c

   Demonstrate the use of setjmp() and longjmp() to perform a nonlocal goto.
*/
#include <setjmp.h>
#include "tlpi_hdr.h"

static jmp_buf env;

static void f2(void) {
    longjmp(env, 1);
}

static void f1(void) {
    f2();
    printf("Returning from f1()\n");  /* NOTREACHED */
}

int main(void) {
    if (setjmp(env) == 0) {   /* Normal return */
        printf("setjmp() returned 0; calling f1()\n");
        f1();
    } else                    /* "Fake" return */
        printf("setjmp() returned nonzero\n");
    exit(EXIT_SUCCESS);
}
