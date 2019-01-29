/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* pshm_seqnum_init.c

   Reset semaphore inside POSIX shared memory object, and optionally
   reinitialize sequence number inside object.

   See also pshm_seqnum_get.c.
*/
#include "pshm_seqnum.h"

int
main(int argc, char *argv[])
{
    int fd;
    struct shmbuf *shmp;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s /shm-name [init-value]\n", argv[0]);

    /* Unlink shared memory name, in case left over by previous run */

    shm_unlink(argv[1]);

    /* Create and size shared memory; size it according to structure
       be placed in memory; map at address chosen by system */

    fd = shm_open(argv[1], O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
        errExit("shm_open");

    if (ftruncate(fd, sizeof(struct shmbuf)) == -1)
        errExit("ftruncate");

    shmp = mmap(NULL, sizeof(struct shmbuf), PROT_READ | PROT_WRITE,
                MAP_SHARED, fd, 0);
    if (shmp == MAP_FAILED)
        errExit("mmap");

    /* Reset semaphore to be available */

    if (sem_init(&shmp->sem, 1, 1) == -1)
        errExit("sem_init");

    /* If argv[2] was specified, reinitialize sequence number */

    if (argc > 2)
        shmp->seqnum = atoi(argv[2]);

    exit(EXIT_SUCCESS);
}
