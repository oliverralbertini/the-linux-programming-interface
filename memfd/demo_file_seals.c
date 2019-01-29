/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* demo_file_seals.c
*/
#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */
#include "tlpi_hdr.h"

/* Glibc doesn't yet provide a wrapper for memfd_create() */

static int
memfd_create(const char *name, unsigned int flags)
{
    return syscall(SYS_memfd_create, name, flags);
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

int
main(int argc, char *argv[])
{
    int fd;
    char *name, *str;
    ssize_t len;

    if (argc < 3)
        usageErr("%s name length string\n", argv[0]);

    name = argv[1];
    len = atoi(argv[2]);
    str = argv[3];

    /* Create an anonymous file in tmpfs; allow seals to be
       placed on the file */

    fd = memfd_create(name, MFD_ALLOW_SEALING);
    if (fd == -1)
        errExit("memfd_create");

    /* Size the file as specified on the command line */

    if (ftruncate(fd, len) == -1)
        errExit("truncate");

    /* Don't allow the file to grow */

    if (fcntl(fd, F_ADD_SEALS, F_SEAL_GROW) == -1)
        errExit("fcntl");

    /* Write string specified on command line to file.  This
       will fail if we write a string greater than the length
       given to the ftruncate() call above. */

    if (write(fd, str, strlen(str)) == -1)
        errExit("write");

    exit(EXIT_SUCCESS);
}
