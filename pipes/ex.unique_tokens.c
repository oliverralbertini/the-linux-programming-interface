/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* unique_tokens.c

   Demonstrate the use of a pipe to connect two filters.

   We execute the command:

       tr ' \t' '\012' < filename | sort -u

   where 'filename' is argv[1].
*/
#include <fcntl.h>
#include <sys/wait.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    int pfd[2];
    int fd;

    if (argc != 2)
        usageErr("%s filename\n", argv[0]);

    /* FIXME: Create pipe used to connect 'tr' and 'sort' */

    /* FIXME: Create a child process that sets up its standard output to be
       the write end of the pipe, opens the file named on the command line
       to be used as standard input, and then executes 'tr' */

    /* FIXME: The parent sets up standard input to be the read end of
       the pipe, and then executes 'sort' */

}
