/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* t_memfd_create.c
*/
#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>

/* Glibc doesn't yet provide a wrapper for memfd_create() */

static int
memfd_create(const char *name, unsigned int flags)
{
    return syscall(__NR_memfd_create, name, flags);
}

/* Glibc headers may not yet have the following definitions */

#ifndef F_ADD_SEALS
#define MFD_CLOEXEC             0x0001U
#define MFD_ALLOW_SEALING       0x0002U

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
    char *name, *seals_arg;
    ssize_t len;

    if (argc < 3) {
        fprintf(stderr, "%s name size [seals]\n", argv[0]);
        fprintf(stderr, "\t'seals' can contain any of the "
                "following characters:\n");
        fprintf(stderr, "\t\tw - F_SEAL_WRITE\n");
        fprintf(stderr, "\t\tg - F_SEAL_GROW\n");
        fprintf(stderr, "\t\ts - F_SEAL_SHRINK\n");
        fprintf(stderr, "\t\tS - F_SEAL_SEAL\n");
        exit(EXIT_FAILURE);
    }

    name = argv[1];
    len = atoi(argv[2]);
    seals_arg = argv[3];

    /* Create an anonymous file in tmpfs; allow seals to be
       placed on the file */

    fd = memfd_create(name, MFD_ALLOW_SEALING);
    if (fd == -1)
        errExit("memfd_create");

    /* Size the file as specified on the command line */

    if (ftruncate(fd, len) == -1)
        errExit("truncate");

    printf("PID: %ld; fd: %d; /proc/%ld/fd/%d\n",
            (long) getpid(), fd, (long) getpid(), fd);

    /* Code to map the file and populate the mapping with data
       omitted */

    /* If a 'seals' command-line argument was supplied, set some
       seals on the file */

    if (seals_arg != NULL) {
        seals = 0;

        if (strchr(seals_arg, 'w') != NULL)
            seals |= F_SEAL_WRITE;
        if (strchr(seals_arg, 'g') != NULL)
            seals |= F_SEAL_GROW;
        if (strchr(seals_arg, 's') != NULL)
            seals |= F_SEAL_SHRINK;
        if (strchr(seals_arg, 'S') != NULL)
            seals |= F_SEAL_SEAL;

        if (fcntl(fd, F_ADD_SEALS, seals) == -1)
            errExit("fcntl");
    }

    /* Keep running, so that the file created by memfd_create()
       continues to exist */

    pause();

    exit(EXIT_SUCCESS);
}
