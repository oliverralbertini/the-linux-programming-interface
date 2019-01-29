/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* sched_nonrealtime.c

   Copyright Michael Kerrisk,  24 Jun 2015

   Demonstrate the effect of setting (nonrealtime) scheduling policies
   and nice values for a process.

   Usage: sched_nonrealtime {o<niceval>|b<niceval>|i}...

   Creates one child process for each command-line argument, setting the
   scheduling policy and priority ("nice value", valid range -20 .. 19)
   of the process according to the argument value.

   The first character of each command-line argument determines the
   scheduling policy of the corresponding child:

        o - SCHED_OTHER
        b - SCHED_BATCH (new in Linux 2.6.16)
        i - SCHED_IDLE  (new in Linux 2.6.23; nice value has no effect
                        for this policy)

   Once created, each child then pauses, until the all of the children are
   started by the parent closing the write end of a pipe from which all
   of the children are trying to read.  Each child then loops infinitely,
   consuming CPU time and updating an entry in a shared-memory array with
   a record of how much time it has so far consumed. The parent periodically
   inspects this array, displaying the amount and percentage of CPU time
   consumed by each child.  In this way, it is possible to see the
   relative amount of CPU time each child process receives, and thus
   the impact on scheduling of different policies and priorities.

   Example usage:

        sudo taskset 0x1 ./sched_nonrealtime o-5 o0

    (The taskset(1) command is used to confine the processes to a single CPU,
    so that they are in competition with one another.)
*/
#define _GNU_SOURCE
#include <time.h>
#include <sys/resource.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sched.h>
#include <limits.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    int childNum, fd, j;
    pid_t childPid;
    double totCpu, usedCpu;
    struct sched_param schedParam;
    int pfd[2];
    char shmPath[PATH_MAX];
    struct timespec *tsList;
    size_t len;
    char ch;

    if (argc < 2)
        usageErr("%s {o<nice-value>|b<nice-value>|i}...\n", argv[0]);

    snprintf(shmPath, PATH_MAX, "/sched_nonrt_%ld", (long) getpid());

    /* Create shared memory array that will be used to record CPU
       usage of each child */

    shm_unlink(shmPath);

    fd = shm_open(shmPath, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
        errExit("shm_open");

    len = (argc - 1) * sizeof(struct timespec);

    if (ftruncate(fd, len) == -1)
        errExit("ftruncate");

    tsList = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (tsList == MAP_FAILED)
        errExit("mmap");

    /* Create a pipe that is used to synchronize the children */

    if (pipe(pfd) == -1)
        errExit("pipe");

    /* Create child processes */

    for (childNum = 1; childNum < argc; childNum++) {
        switch (childPid = fork()) {
        case -1:
            errExit("fork");

        case 0:
            close(pfd[1]);      /* Children don't need write end of pipe */

            /* Child sets its scheduling policy based
               on corresponding command-line argument */

            switch (argv[childNum][0]) {
            case 'o':
                /* SCHED_OTHER is the default policy, so there's
                   nothing to do here */
                break;

#ifdef SCHED_BATCH      /* Linux-specific */
            case 'b':
                schedParam.sched_priority = 0;
                if (sched_setscheduler(0, SCHED_BATCH, &schedParam) == -1)
                    errExit("sched_setscheduler - childNum=%d",
                            childNum);
                break;
#endif

#ifdef SCHED_IDLE       /* Linux-specific */
            case 'i':
                schedParam.sched_priority = 0;
                if (sched_setscheduler(0, SCHED_IDLE, &schedParam) == -1)
                    errExit("sched_setscheduler - childNum=%d",
                            childNum);
                break;
#endif

            default:
                fatal("bad policy: %c", argv[childNum][0]);
            }

            /* SCHED_OTHER and SCHED_BATCH employ the nice value;
               get the value from the command-line argument */

            if (argv[childNum][0] == 'o' || argv[childNum][0] == 'b')
                if (setpriority(PRIO_PROCESS, 0,
                         atoi(&argv[childNum][1])) == -1)
                    errExit("setpriority");

            printf("Child %d (PID=%ld) prio=%d\n", childNum,
                    (long) getpid(), getpriority(PRIO_PROCESS, 0));

            /* Read from the pipe. This will cause the child to pause
               until the parent closes the write end of the pipe. */

            read(pfd[0], &ch, 1);

            /* Loop burning up CPU time */

            for (;;) {
                for (j = 0; j < 1000; )
                    j++;

                /* Place CPU time consumed so far into shared memory array */

                if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID,
                        &tsList[childNum - 1]) == -1)
                    errExit("clock_gettime");
            }

            exit(EXIT_SUCCESS);

        default:        /* Parent */
            usleep(10000);      /* Delay briefly before creating next
                                   child (so that child creation messages
                                   appear in an orderly fashion) */
            break;
        }
    }

    /* Parent falls through to here */

    printf("\n");

    shm_unlink(shmPath);        /* Shared memory path is no longer needed */

    /* Parent gives itself a high nice value, so that it gets CPU in
       a timely fashion */

    if (setpriority(PRIO_PROCESS, 0, -20) == -1)
        errExit("setpriority");

    /* Start the children */

    close(pfd[1]);

    /* Periodically inspect the shared memory array containing the
       amount of CPU time consumed by each child, and print out info
       for each child, including percentage of CPU time consumed */

    for (;;) {
        sleep(1);
        totCpu = 0.0;

        /* Yes, the following code is racy. It might happen that an
           element of the array is changed while we perform the steps
           below, with the consequence that we may see incorrect
           statistics occasionally. */

        for (j = 0; j < argc -1; j++)
            totCpu += tsList[j].tv_sec + tsList[j].tv_nsec / 1000000000.0;

        printf("Total CPU = %.3f: ", totCpu);

        for (j = 0; j < argc -1; j++) {
            usedCpu = tsList[j].tv_sec + tsList[j].tv_nsec / 1000000000.0;
            printf("  [%.3f %.2f%%]", usedCpu, usedCpu / totCpu * 100.0);
        }

        printf("\n");
    }
}
