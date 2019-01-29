/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* rtsched.c

   Execute a command under a realtime scheduling policy and priority.
   This program should be set up as set-user-ID-root.
*/
#define _GNU_SOURCE
#include <sched.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    int policy;
    struct sched_param param;

    if (argc < 4 || strcmp(argv[1], "--help") == 0)
        usageErr("%s [r|f] <prio> <cmd> <arg>...\n", argv[0]);

    if (argv[1][0] == 'r')
        policy = SCHED_RR;
    else if (argv[1][0] == 'f')
        policy = SCHED_FIFO;
    else
        fatal("Policy must be 'f' or 'r'");

    param.sched_priority = atoi(argv[2]);

    /* FIXME: Set the scheduling policy and priority of the process */

    /* FIXME: This program should have been set up as a set-UID-root program;
       reset all UIDs to real UID before executing the command */

    /* FIXME: Execute the command supplied on the command line */

}
