/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* t_add_seals.c
*/
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Glibc headers may not yet have the following definitions */

#ifndef F_ADD_SEALS
#define F_LINUX_SPECIFIC_BASE 1024
#define F_ADD_SEALS     (F_LINUX_SPECIFIC_BASE + 9)
#define F_GET_SEALS     (F_LINUX_SPECIFIC_BASE + 10)

#define F_SEAL_SEAL     0x0001  /* prevent further seals from being set */
#define F_SEAL_SHRINK   0x0002  /* prevent file from shrinking */
#define F_SEAL_GROW     0x0004  /* prevent file from growing */
#define F_SEAL_WRITE    0x0008  /* prevent writes */
#endif

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                        } while (0)

int
main(int argc, char *argv[])
{
    int fd;
    unsigned int seals;

    if (argc < 2) {
        fprintf(stderr, "%s /proc/PID/fd/FD [seals]\n", argv[0]);
        fprintf(stderr, "\t'seals' can contain any of the "
                "following characters:\n");
        fprintf(stderr, "\t\tw - F_SEAL_WRITE\n");
        fprintf(stderr, "\t\tg - F_SEAL_GROW\n");
        fprintf(stderr, "\t\ts - F_SEAL_SHRINK\n");
        fprintf(stderr, "\t\tS - F_SEAL_SEAL\n");
        exit(EXIT_FAILURE);
    }

    fd = open(argv[1], O_RDWR);
    if (fd == -1)
        errExit("open");

    if (argc > 2) {
        seals = 0;

        if (strchr(argv[2], 'w') != NULL)
            seals |= F_SEAL_WRITE;
        if (strchr(argv[2], 'g') != NULL)
            seals |= F_SEAL_GROW;
        if (strchr(argv[2], 's') != NULL)
            seals |= F_SEAL_SHRINK;
        if (strchr(argv[2], 'S') != NULL)
            seals |= F_SEAL_SEAL;

        if (fcntl(fd, F_ADD_SEALS, seals) == -1)
            errExit("fcntl");
    }

    exit(EXIT_SUCCESS);
}
