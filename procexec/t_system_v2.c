/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* t_system_v2.c

   Demonstrate the use of system(3) to execute a shell command.
*/
#include <sys/wait.h>
#include "print_wait_status.h"
#include "tlpi_hdr.h"

#define MAX_CMD_LEN 200

int
main(int argc, char *argv[])
{
    char str[MAX_CMD_LEN];
    int status;

    for (;;) {      /* Read and execute a shell command */
        printf("Command>>> ");
        fflush(stdout);
        if (fgets(str, MAX_CMD_LEN, stdin) == NULL)
            break;              /* end-of-file */

        status = system(str);

        if (status == -1)
            errExit("system");
        if (WIFEXITED(status) && WEXITSTATUS(status) == 127)
            printf("Could not invoke shell\n");
        else    /* Successfully executed command */
            printWaitStatus("Command status: ", status);
    }

    exit(EXIT_SUCCESS);
}
