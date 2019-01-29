/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* prod_cons_cv.c

   A simple POSIX threads producer-consumer example using a condition variable.
*/
#include <time.h>
#include <pthread.h>
#include "tlpi_hdr.h"

static int sFactor = 1000;

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t units_avail = PTHREAD_COND_INITIALIZER;

/* The following globals are protected by 'mtx' */

static int avail = 0;           /* Number of unconsumed units */
static int totalUnits;          /* Total number of units that
                                   all threads will produce */
static int unitsProduced;       /* Total units so far produced */
static int unitsConsumed;       /* Total units so far consumed */
static int numLiveCons;         /* Number of live consumers */

static void *
producer(void *arg)
{
    int s;
    int prodDone;    /* Will be set true when all units have been
                        produced. This will then cause the main loop
                        below to terminate--after unlocking the mutex. */
    prodDone = 0;

    /* Loop to produce units */

    while (!prodDone) {

        /* Code to produce a unit omitted */

        if (sFactor > 0)
            usleep(100 * sFactor);      /* Simulate work required
                                           to produce a unit */
        s = pthread_mutex_lock(&mtx);
        if (s != 0)
            errExitEN(s, "pthread_mutex_lock");

        prodDone = unitsProduced >= totalUnits;

        if (!prodDone) {

            /* Produce a unit */

            avail++;
            unitsProduced++;
            if (sFactor > 0) {
                printf("Produced a unit (total produced: %d): avail = %d\n",
                        unitsProduced, avail);
                usleep(10 * sFactor);   /* Simulate work required to store
                                           unit in data structure */
            }
        }

        s = pthread_mutex_unlock(&mtx);
        if (s != 0)
            errExitEN(s, "pthread_mutex_unlock");

        /* Wake sleeping consumers */

        /* We must use pthread_cond_broadcast() here, not
           pthread_cond_signal(), so that all consumers are
           forced out of pthread_cond_wait)() to recheck whether
           (unitsConsumed < totalUnits). */

        s = pthread_cond_broadcast(&units_avail);
        if (s != 0)
            errExitEN(s, "pthread_cond_broadcast");
    }

    if (sFactor > 0)
        printf("\t\t\t\t Producer terminating\n");

    return NULL;
}

static void *
consumer(void *arg)
{
    int s;
    int consDone;    /* Will be set true when all units have been
                        consumed. This will then cause the main loop
                        below to terminate--after unlocking the mutex. */
    consDone = 0;

    /* Loop to consume units */

    while (!consDone) {
        s = pthread_mutex_lock(&mtx);
        if (s != 0)
            errExitEN(s, "pthread_mutex_lock");

        /* Wait for something to consume.

           We should wait on the condition variable only if
           there are no units available *and* units are still
           being produced (unitsConsumed < totalUnits). */

        while (avail == 0 && unitsConsumed < totalUnits) {
            s = pthread_cond_wait(&units_avail, &mtx);
            if (s != 0)
                errExitEN(s, "pthread_cond_wait");
        }

        /* At this point, 'mtx' is locked... */

        if (unitsConsumed >= totalUnits) {

            /* If all units have been consumed, our work is done */

            numLiveCons--;
            consDone = 1;

            if (numLiveCons <= 0) {

                /* We are the last consumer. Terminate the process. */

                printf("Produced: %d; consumed: %d. ",
                        unitsProduced, unitsConsumed);
                printf("All units consumed; terminating\n");

                exit(EXIT_SUCCESS);
            }

        } else {

            /* Consume a unit */

            unitsConsumed++;
            avail--;

            if (sFactor > 0) {
                printf("Consumed a unit (total consumed: %d); avail = %d\n",
                        unitsConsumed, avail);

                usleep(50 * sFactor);   /* Simulate work required to remove
                                           unit from data structure */
            }
        }

        s = pthread_mutex_unlock(&mtx);
        if (s != 0)
            errExitEN(s, "pthread_mutex_unlock");

        /* Do some work that does not require the mutex lock */

        if (sFactor > 0)
            usleep(500 * sFactor);      /* Simulate doing other work */

    }

    if (sFactor > 0)
        printf("\t\t\t\t Consumer terminating\n");

    return NULL;
}

int
main(int argc, char *argv[])
{
    pthread_t tid;
    int s, j;
    int numCons, numProd;

    if (argc < 4) {
        fprintf(stderr,
                "Usage: %s num-prod num-cons num-units"
                " [sleep-factor]\n\n", argv[0]);
        fprintf(stderr, "\t'num-prod' is the number of producer threads\n");
        fprintf(stderr, "\t'num-cons' is the number of consumer threads\n");
        fprintf(stderr, "\t'num-units' is the total number of units to "
                "produce and consume\n");
        fprintf(stderr, "\t'sleep-factor' is a duration multiplier "
                "for sleeps that simulate work.\n");
        fprintf(stderr, "\t\tA value of zero disables sleeps "
                "and informational messages.\n");
        fprintf(stderr, "\t\tThe default value is %d.\n", sFactor);
        exit(EXIT_FAILURE);
    }

    numProd = atoi(argv[1]);
    numLiveCons = numCons = atoi(argv[2]);

    totalUnits = atoi(argv[3]);
    unitsConsumed = 0;

    if (argc > 4)
        sFactor = atoi(argv[4]);

    /* Create all producer threads */

    for (j = 0; j < numProd; j++) {
        s = pthread_create(&tid, NULL, producer, NULL);
        if (s != 0)
            errExitEN(s, "pthread_create");
    }

    /* Create all consumer threads */

    for (j = 0; j < numCons; j++) {
        s = pthread_create(&tid, NULL, consumer, NULL);
        if (s != 0)
            errExitEN(s, "pthread_create");
    }

    /* Program will terminate when last consumer calls exit() */

    pause();
}
