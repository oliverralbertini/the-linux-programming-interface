/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* t_clock_nanosleep_abstime.c

   Demonstrate the use of clock_nanosleep() to sleep for an interval
   specified in seconds and nanoseconds. We use TIMER_ABSTIME to
   avoid the "creeping sleep" problem.

   See also t_nanosleep.c.
*/
#define _XOPEN_SOURCE 600
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include "tlpi_hdr.h"

static void
sigintHandler(int sig)
{
    return;                     /* Just interrupt clock_nanosleep() */
}

int
main(int argc, char *argv[])
{
    struct timespec request, start, curr;
    struct sigaction sa;
    int s;

    if (argc < 2)
        usageErr("%s secs [nanosecs]\n", argv[0]);

    /* Allow SIGINT handler to interrupt clock_nanosleep() */

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigintHandler;
    if (sigaction(SIGINT, &sa, NULL) == -1)
        errExit("sigaction");

    if (clock_gettime(CLOCK_MONOTONIC, &start) == -1)
        errExit("clock_gettime");
    request = start;

    /* Calculate absolute time value for clock_nanosleep() */

    request.tv_sec  += atoi(argv[1]);
    request.tv_nsec += (argc > 2) ? atoi(argv[2]) : 0;

    if (request.tv_nsec >= 1000000000) {
        request.tv_sec += request.tv_nsec / 1000000000;
        request.tv_nsec %= 1000000000;
    }

    for (;;) {
        s = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME,
                            &request, NULL);
        if (s != 0 && s != EINTR)
            errExitEN(s, "clock_nanosleep");

        if (clock_gettime(CLOCK_MONOTONIC, &curr) == -1)
            errExit("clock_gettime");
        printf("Slept: %.6f secs", curr.tv_sec - start.tv_sec +
                        (curr.tv_nsec - start.tv_nsec) / 1000000000.0);

        if (s == 0)
            break;                      /* sleep completed */

        printf("... restarting\n");
    }

    printf("\nSleep complete\n");
    exit(EXIT_SUCCESS);
}
