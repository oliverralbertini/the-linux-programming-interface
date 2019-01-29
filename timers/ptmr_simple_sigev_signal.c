/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* ptmr_simple_sigev_signal.c

   This program demonstrates the use of signals as the notification
   mechanism for expirations of a POSIX timer. The program's command-line
   arguments specify the initial value and interval for a POSIX timer.

   The program establishes a handler for the timer notification signal,
   creates and arms the timer, and then pauses. Each timer expiration
   causes the generation of a signal, and, when invoked, the signal
   handler displays some information about the timer expiration.
*/
#define _POSIX_C_SOURCE 199309
#include <signal.h>
#include <time.h>
#include "curr_time.h"                  /* Declares currTime() */
#include "tlpi_hdr.h"

static void
handler(int sig, siginfo_t *si, void *uc)
{
    timer_t *tidptr = si->si_value.sival_ptr;

    /* UNSAFE: This handler uses non-async-signal-safe functions
       (printf(); see Section 21.1.2) */

    printf("[%s] Got signal %d\n", currTime("%T"), sig);
    printf("    timer ID = %ld\n", (long) *tidptr);
    printf("    overruns = %d\n", timer_getoverrun(*tidptr));
}

int
main(int argc, char *argv[])
{
    struct itimerspec ts;
    struct sigaction  sa;
    struct sigevent   sev;
    timer_t tid;

    if (argc < 2)
        usageErr("%s secs [nsecs [int-secs [int-nsecs]]]\n",
                argv[0]);

    ts.it_value.tv_sec = atoi(argv[1]);
    ts.it_value.tv_nsec = (argc > 2) ? atoi(argv[2]) : 0;
    ts.it_interval.tv_sec = (argc > 3) ? atoi(argv[3]) : 0;
    ts.it_interval.tv_nsec = (argc > 4) ? atoi(argv[4]) : 0;

    /* Establish handler for notification signal */

    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGRTMAX, &sa, NULL) == -1)
        errExit("sigaction");

    sev.sigev_notify = SIGEV_SIGNAL;    /* Notify via signal */
    sev.sigev_signo = SIGRTMAX;         /* Notify using this signal */
    sev.sigev_value.sival_ptr = &tid;   /* Pass timer ID to handler */

    if (timer_create(CLOCK_REALTIME, &sev, &tid) == -1)
        errExit("timer_create");
    printf("Timer ID: %ld\n", (long) tid);

    if (timer_settime(tid, 0, &ts, NULL) == -1)
        errExit("timer_settime");

    for (;;)            /* Wait for incoming timer signals */
        pause();
}
