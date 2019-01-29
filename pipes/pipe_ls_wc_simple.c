/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* pipe_ls_wc_simple.c

   Demonstrate the use of a pipe to connect two filters. We use fork()
   to create a child. The child execs ls(1), which writes to the pipe,
   while the parent execs wc(1) to read from the pipe.
*/
#include <sys/wait.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    int pfd[2];                         /* Pipe file descriptors */

    if (pipe(pfd) == -1)                /* Create pipe */
        errExit("pipe");

    switch (fork()) {
    case -1:
        errExit("fork");

    case 0:     /* Child: exec 'ls' to write to pipe */
        if (close(pfd[0]) == -1)        /* Read end is unused */
            errExit("close 1");

        /* Duplicate stdout on write end of pipe */

        if (pfd[1] != STDOUT_FILENO) {  /* Defensive check */
            if (dup2(pfd[1], STDOUT_FILENO) == -1)
                errExit("dup2 1");
            if (close(pfd[1]) == -1)
                errExit("close 2");
        }

        execlp("ls", "ls", (char *) NULL);
        errExit("execlp ls");

    default:    /* Parent: exec 'wc -l' to read from pipe */
        if (close(pfd[1]) == -1)        /* Write end is unused */
            errExit("close 3");

        /* Duplicate stdin on read end of pipe */

        if (pfd[0] != STDIN_FILENO) {   /* Defensive check */
            if (dup2(pfd[0], STDIN_FILENO) == -1)
                errExit("dup2 2");
            if (close(pfd[0]) == -1)
                errExit("close 4");
        }

        execlp("wc", "wc", "-l", (char *) NULL);
        errExit("execlp wc");
    }
}
