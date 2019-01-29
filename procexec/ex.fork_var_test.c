/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* fork_var_test.c

   Demonstrate the use of fork(), showing that parent and child
   get separate copies of variables.
*/
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    pid_t childPid;
    int ivar = 99;

    /* FIXME: Use fork() to create a child. After the fork(), the parent should
       sleep briefly, while the child modifies the value of 'ivar' */

    /* FIXME: Both parent and child should print their PID and
       the value of the 'ivar' variable */

    exit(EXIT_SUCCESS);
}
