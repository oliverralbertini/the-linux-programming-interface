/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* exec_cnt.c
*/
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    char *valp;
    int execCnt;

    /* FIXME: Add additional variable declarations, as needed */

    printf("PID = %ld\n", (long) getpid());

    /* Fetch and display the value of the environment variable EXEC_CNT */

    valp = getenv("EXEC_CNT");
    if (valp == NULL)
        execCnt = 0;
    else
        execCnt = atoi(valp);

    printf("EXEC_CNT is %d\n\n", execCnt);

    /* FIXME: Update EXEC_CNT in the environment so that its value is
       a string that is an integer one greater than previously */

    /* FIXME: If command-line arguments were supplied, execute the file
       named in argv[1], using the command-line arguments argv[1]... as
       the argument vector to the new program. The caller should pass
       its own environment as the environment for the new program. */

    exit(EXIT_SUCCESS);
}
