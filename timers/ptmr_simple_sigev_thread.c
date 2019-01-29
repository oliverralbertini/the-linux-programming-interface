/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* ptmr_simple_sigev_thread.c

*/
#include <signal.h>
#include <time.h>
#include <pthread.h>
#include "curr_time.h"          /* For decln of currTime() */
#include "tlpi_hdr.h"

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static int expireCnt = 0;  /* Counts timer expirations */

static void             /* Thread notification function */
threadFunc(union sigval sv)
{
    timer_t *tidptr = sv.sival_ptr;
    int s;

    printf("[%s] Thread notify\n", currTime("%T"));
    printf("    timer ID = %ld; overrun = %d\n",
            (long) *tidptr, timer_getoverrun(*tidptr));

    /* Increment counter variable shared with main thread and signal
       condition variable to notify main thread of the change. */

    s = pthread_mutex_lock(&mtx);
    if (s != 0)
        errExitEN(s, "pthread_mutex_lock");

    expireCnt += 1 + timer_getoverrun(*tidptr);

    s = pthread_mutex_unlock(&mtx);
    if (s != 0)
        errExitEN(s, "pthread_mutex_unlock");

    s = pthread_cond_signal(&cond);
    if (s != 0)
        errExitEN(s, "pthread_cond_signal");
}

int
main(int argc, char *argv[])
{
    struct sigevent sev;
    struct itimerspec ts;
    timer_t tid;
    int numExpire, s;

    if (argc < 3)
        usageErr("%s num-expire secs [nsecs [int-secs [int-nsecs]]]\n",
                argv[0]);

    sev.sigev_notify = SIGEV_THREAD;        /* Notify via thread */
    sev.sigev_notify_function = threadFunc; /* Thread start function */
    sev.sigev_value.sival_ptr = &tid;
            /* Passed as argument to threadFunc() */
    sev.sigev_notify_attributes = NULL;
            /* Could be pointer to pthread_attr_t structure */

    if (timer_create(CLOCK_REALTIME, &sev, &tid) == -1)
        errExit("timer_create");
    printf("main(): timer ID = %ld %s\n", (long) tid, currTime("%T"));

    numExpire = atoi(argv[1]);

    ts.it_value.tv_sec = atoi(argv[2]);
    ts.it_value.tv_nsec = (argc > 3) ? atoi(argv[3]) : 0;
    ts.it_interval.tv_sec = (argc > 4) ? atoi(argv[4]) : 0;
    ts.it_interval.tv_nsec = (argc > 5) ? atoi(argv[5]) : 0;

    if (timer_settime(tid, 0, &ts, NULL) == -1)
        errExit("timer_settime");

    /* Loop until thread notification function has been called
       'numExpire' times */

    s = pthread_mutex_lock(&mtx);
    if (s != 0)
        errExitEN(s, "pthread_mutex_lock");

    while (expireCnt < numExpire) {
        s = pthread_cond_wait(&cond, &mtx);
        if (s != 0)
            errExitEN(s, "pthread_cond_wait");
        printf("main(): count = %d\n", expireCnt);
    }

    printf("All done!\n");
    exit(EXIT_SUCCESS);
}
