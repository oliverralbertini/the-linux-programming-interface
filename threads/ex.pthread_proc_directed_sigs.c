/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* pthread_proc_directed_sigs.c

   A program to show that in a multithreaded process, signals are
   caught (only) in the threads that are not blocking the signals.
*/
#include <signal.h>
#include <pthread.h>
#include "tlpi_hdr.h"

static int handlingThreads;     /* Number of still running threads
                                   that are handling SIGQUIT */
static pthread_mutex_t mtxHandling = PTHREAD_MUTEX_INITIALIZER;
                                /* Protects 'handlingThreads' */

static pthread_cond_t threadTerminated = PTHREAD_COND_INITIALIZER;

static int maxsigs;             /* Maximum number of signals handled by
                                   each thread (taken from argv[1]) */

/* Structure passed as argument to the thread start function */

struct threadArg {
    int tnum;           /* Thread number */
    int handle;         /* Nonzero if thread should unblock and
                           handle SIGQUIT */
};

static void             /* SIGQUIT handler */
sigquitHandler(int sig)
{
    /* Does nothing but interrupt calls to pause() */
}

static void *
threadFunc(void *arg)
{
    struct threadArg *ta = arg;
    int sigcnt = 0;     /* Number of signals caught in this thread */

    /* FIXME: Other variable declarations as needed */

    /* FIXME: We inherited a signal mask from the main thread. If specified in
       the corresponding command-line argument, unblock SIGQUIT so that it may
       be handled in this thread. */

    /* FIXME: Loop waiting for signals to be delivered. Each time pause()
       returns, increment the count of signals and print a message.
       Exit the loop when the number of signals caught reaches 'maxsigs'. */

    /* FIXME: Decrement 'handlingThreads' under mutex control */

    /* FIXME: Signal the 'threadTerminated' condition variable, so that
       the main thread rechecks the value of 'handlingThreads' */

    /* Say bye bye */

    printf("***** Thread %d terminating\n", ta->tnum);
    free(ta);
    return NULL;
}

int
main(int argc, char *argv[])
{
    int j, s;
    pthread_t thr;
    struct threadArg *ta;
    sigset_t mask;
    struct sigaction sa;

    if (argc < 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s maxsigs [xB] [xB]...\n", argv[0]);

    maxsigs = atoi(argv[1]);

    /* FIXME: Block SIGQUIT */

    /* FIXME: Establish a handler for SIGQUIT */

    /* Lock the mutex before creating any threads, so that no child may
       signal the condition variable before we call pthread_cond_wait() */

    pthread_mutex_lock(&mtxHandling);

    /* Create one thread for each command-line argument from argv[2] onward */

    for (j = 2; j < argc; j++) {

        /* Allocate and initialize a 'threadArg' structure to contain
           a sequential thread number (starting at 1 for the first thread)
           and a flag to indicate whether the thread should keep SIGQUIT
           blocked. */

        ta = malloc(sizeof(struct threadArg));
        if (ta == NULL)
            errExit("malloc");

        ta->tnum = j - 1;
        ta->handle = argv[j][0] != 'B';

        /* If the thread should not block, increment 'handlingThreads' */

        if (ta->handle)
            handlingThreads++;

        /* Create the thread, passing the 'threadArg' structure as the
           argument of the thread start function (threadFunc()) */

        s = pthread_create(&thr, NULL, threadFunc, ta);
        if (s != 0)
            errExitEN(s, "pthread_create");
    }

    /* FIXME: So long as 'handlingThreads' is not 0, wait on the condition variable
       'threadTerminated' */

    /* Once 'handlingThreads' has reached 0, we can terminate the process */

    printf("Bye!\n");
    exit(EXIT_SUCCESS);
}
