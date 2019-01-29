/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* child_exec.c

   A simple example of the use of fork(), execve(), exit(), and
   waitpid().
*/
#include <sys/wait.h>
#include "print_wait_status.h"
#include "tlpi_hdr.h"

extern char **environ;

int
main(int argc, char *argv[])
{
    pid_t childPid, wpid;
    int wstatus;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s command [arg...]\n", argv[0]);

    switch (childPid = fork()) {
    case -1:
        errExit("fork");
    case 0:     /* Child */
        printf("PID of child: %ld\n", (long) getpid());
        execve(argv[1], &argv[1], environ);
        errExit("execve");
    default:    /* Parent */
        wpid = waitpid(childPid, &wstatus, 0);
        if (wpid == -1)
            errExit("waitpid");
        printf("Wait returned PID %ld\n", (long) wpid);
        printWaitStatus("        ", wstatus);
    }

    exit(EXIT_SUCCESS);
}
