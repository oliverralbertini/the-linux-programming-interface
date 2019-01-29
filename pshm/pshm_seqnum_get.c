/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* pshm_seqnum_get.c

   Fetch and display current value of sequence number in POSIX shared
   memory object, and optionally add a value to sequence number.

   See also pshm_seqnum_init.c.
*/
#include "pshm_seqnum.h"

int
main(int argc, char *argv[])
{
    int fd, runLength;
    struct shmbuf *shmp;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s /shm-name [run-length]\n", argv[0]);

    /* Opening existing shared memory object;
       map at address chosen by system */

    fd = shm_open(argv[1], O_RDWR, 0);
    if (fd == -1)
        errExit("shm_open");

    shmp = mmap(NULL, sizeof(struct shmbuf), PROT_READ | PROT_WRITE,
                MAP_SHARED, fd, 0);
    if (shmp == MAP_FAILED)
        errExit("mmap");

    /* Reserve the semaphore while accessing sequence number */

    if (sem_wait(&shmp->sem) == -1)
        errExit("sem_init");

    printf("Current value of sequence number: %d\n", shmp->seqnum);

    /* If a second command-line argument (a number) was supplied,
       add that number to the sequence number */

    if (argc > 2) {
        runLength = atoi(argv[2]);
        if (runLength <= 0)
            fprintf(stderr, "Invalid run-length\n");
        else {
            sleep(3);           /* Make update slow */
            shmp->seqnum += runLength;
            printf("Updated sequence number\n");
        }
    }

    if (sem_post(&shmp->sem) == -1)
        errExit("sem_init");

    exit(EXIT_SUCCESS);
}
