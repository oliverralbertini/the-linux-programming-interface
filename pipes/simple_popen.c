/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* simple_popen.c

   A simple example of the use of popen() and pclose().
   Execute a command that outputs selected fields of a directory
   listing and display the result on stdout.
*/
#include <limits.h>
#include "print_wait_status.h"
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    const int BUF_SIZE = (PATH_MAX + 50);
    char buf[BUF_SIZE];
    int fcnt, wstatus;
    FILE *fp;

    fp = popen("ls -l *.log --time-style='+%F %T' "
               "2> /dev/null | cut -c33-", "r");
    if (fp == NULL)
        errExit("popen");

    fcnt = 0;
    while (fgets(buf, BUF_SIZE, fp) != NULL) {
        printf("%s", buf);
        fcnt++;
    }
    printf("%d files found\n", fcnt);

    wstatus = pclose(fp);
    printWaitStatus("Status from pclose(): ", wstatus);

    exit(EXIT_SUCCESS);
}
