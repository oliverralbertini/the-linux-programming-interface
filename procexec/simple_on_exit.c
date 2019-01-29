/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* simple_on_exit.c

*/
#define _BSD_SOURCE
#include <stdlib.h>
#include "tlpi_hdr.h"

static void
exitFunc(int status, void *arg)
{
    printf("exitFunc: status = %d, arg = %s\n", status, (char *) arg);
}

int
main(int argc, char *argv[])
{
    if (on_exit(exitFunc, (void *) "hello") != 0)
        errExit("on_exit");
    if (on_exit(exitFunc, (void *) "world") != 0)
        errExit("on_exit");
    exit((argc > 1) ? atoi(argv[1]) : EXIT_FAILURE);
}
