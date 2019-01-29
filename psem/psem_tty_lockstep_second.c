/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* psem_tty_lockstep_second.c

   Use a pair of named POSIX semaphores to ensure that this program
   and its peer (psem_tty_lockstep_init.c) have alternating lockstep
   access to display messages on the terminal.

   The psem_tty_lockstep_init.c program must be run first.
*/
#include <semaphore.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    sem_t *initSem, *secondSem;
    int nloops;

    if (argc < 3 || strcmp(argv[optind], "--help") == 0)
        usageErr("%s /init-sem /second-sem [num-loops\n", argv[0]);

    nloops = (argc > 3) ? atoi(argv[3]) : 5;

    /* Open existing semaphore objects */

    initSem = sem_open(argv[1], 0);
    if (initSem == SEM_FAILED)
        errExit("sem_open-initSem");

    secondSem = sem_open(argv[2], 0);
    if (secondSem == SEM_FAILED)
        errExit("sem_open-secondSem");

    /* Alternate terminal access with initial process */

    for (int j = 0; j < nloops; j++) {
        if (sem_wait(secondSem) == -1)  /* Wait for our turn */
            errExit("sem_wait");

        printf("bbbb %d\n", j);
        usleep(250000);                 /* Go slowly... */

        if (sem_post(initSem) == -1)    /* Give initial process a turn */
            errExit("sem_post");
    }

    exit(EXIT_SUCCESS);
}
