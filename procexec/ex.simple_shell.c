/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* simple_shell.c

   Implement a simple shell-type program. This programs reads line
   of input, each of which contains a command + optional arguments,
   and then executes those commands via fork() and execve().
*/
#include <ctype.h>
#include <sys/wait.h>
#include "print_wait_status.h"
#include "tlpi_hdr.h"

#define MAX_LINE_LEN 10000
#define MAX_ARG 1000

extern char **environ;

int
main(int argc, char *argv[])
{
    char *argVec[MAX_ARG + 1];
    char line[MAX_LINE_LEN];
    int argCnt;

    /* FIXME: Further variable declarations required */

    for (;;) {

        /* Print a prompt, and read a line of input */

        printf("Command>> ");
        fflush(stdout);         /* Ensure prompt is flushed to terminal */

        if (fgets(line, MAX_LINE_LEN, stdin) == NULL)
            break;              /* Exit loop on end of file */

        /* Break the line into arguments delimited by white space.
           strtok() makes the job simple, and it automatically inserts
           a null terminator at the end of each token that it finds.
           On the first call to strtok(), the first argument specifies
           the string to be parsed; for each subsequent call that
           should parse the same string, we specify the first argument
           as NULL. When it finds no more tokens, strtok() returns NULL,
           which we conveniently use to ensure that a NULL pointer is
           placed at the end of the pointer list in 'argVec'. */

        for (argCnt = 0; argCnt <= MAX_ARG; argCnt++) {
            argVec[argCnt] = strtok((argCnt == 0) ? line : NULL, " \t\n\r");
            if (argVec[argCnt] == NULL)
                break;
        }

        if (argCnt == 0)        /* Empty command line */
            continue;

        if (argCnt > MAX_ARG) {
            fprintf(stderr, "Too many words (> %d) in command line\n",
                    MAX_ARG);
            continue;
        }

        /* At this point, 'argVec' contains 'argCnt' pointers to
           null-terminated tokens in 'line', and 'argVec[argCnt]'
           is NULL. */

        /* FIXME: Create a child to execute the command whose pathname
           and arguments are contained in 'argVec' */

    }

    printf("\n");       /* So that shell prompt appears on a new line */
    exit(EXIT_SUCCESS);
}
