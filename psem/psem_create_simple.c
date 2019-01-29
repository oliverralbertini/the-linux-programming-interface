/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* psem_create_simple.c

   Create a POSIX named semaphore.

   On Linux, named semaphores are supported with kernel 2.6 or later, and
   a glibc that provides the NPTL threading implementation.

   This is a simplified version of psem_create.c.
*/
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    sem_t *sem;
    int value;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s /sem-name [init-value]\n", argv[0]);

    value = (argc > 2) ? atoi(argv[2]) : 0;

    sem = sem_open(argv[1], O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, value);
    if (sem == SEM_FAILED)
        errExit("sem_open");

    exit(EXIT_SUCCESS);
}
