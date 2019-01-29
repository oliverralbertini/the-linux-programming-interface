/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* cpu_watchdog.c
*/
#include <time.h>
#include <signal.h>
#include "tlpi_hdr.h"

/* FIXME: Implement a handler for SIGRTMAX */

int
main(int argc, char *argv[])
{
    int killSecs, watchSecs;
    pid_t pid;

    /* FIXME: Further variable declarations */

    if (argc < 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s PID watch-secs [kill-secs]\n", argv[0]);

    pid = atoi(argv[1]);
    watchSecs = atoi(argv[2]);
    killSecs = (argc > 3)? atoi(argv[3]) : 0;

    /* FIXME: Get CPU clock ID of the process specified in command line */

    /* FIXME: Establish handler for timer notification signal (SIGRTMAX) */

    /* FIXME: Create a timer that counts the CPU time used by the target
       process and delivers SIGRTMAX on expirations of the timer */

    /* FIXME: Arm the timer, with the first expiration after one second,
       and then at each second thereafter */

    /* FIXME: Loop, pausing for timer expirations, reporting the time every
       'watchSec' seconds and (if 'killSecs' is nonzero), sending a SIGTERM
       signal after the process has consumed 'killSecs' of CPU time */

    exit(EXIT_SUCCESS);
}
