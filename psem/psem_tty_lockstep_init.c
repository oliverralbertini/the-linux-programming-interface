/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* psem_tty_lockstep_init.c

   Use a pair of named POSIX semaphores to ensure that this program
   and its peer (psem_tty_lockstep_second.c) have alternating lockstep
   access to display messages on the terminal.

   This program must be run before psem_tty_lockstep_second.c, since it
   creates and initializes the semaphores used by both programs.
*/
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    sem_t *initSem, *secondSem;
    int nloops;

    if (argc < 3 || strcmp(argv[optind], "--help") == 0)
        usageErr("%s /init-sem /second-sem [num-loops]\n", argv[0]);

    nloops = (argc > 3) ? atoi(argv[3]) : 5;

    /* Delete old semaphore instances, in case they exist */

    sem_unlink(argv[1]);
    sem_unlink(argv[2]);

    /* Create and initialize the semaphores so that second semaphore
       is available, and our semaphore is not. As a result, the
       second process has first access to the terminal. */

    initSem = sem_open(argv[1], O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0);
    if (initSem == SEM_FAILED)
        errExit("sem_open-initSem");

    secondSem = sem_open(argv[2], O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);
    if (secondSem == SEM_FAILED)
        errExit("sem_open-secondSem");

    /* Alternate terminal access with second process */

    for (int j = 0; j < nloops; j++) {
        if (sem_wait(initSem) == -1)    /* Wait for our turn */
            errExit("sem_wait");

        printf("aaaa %d\n", j);

        usleep(250000);                 /* Go slowly... */

        if (sem_post(secondSem) == -1)  /* Give second process a turn */
            errExit("sem_post");
    }

    sem_unlink(argv[1]);
    sem_unlink(argv[2]);

    exit(EXIT_SUCCESS);
}
