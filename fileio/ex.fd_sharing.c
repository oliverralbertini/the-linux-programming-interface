/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* fd_sharing.c

   Demonstrate that duplicate file descriptors share the file
   offset and file status flags by writing a program that:

   * Opens a file and duplicates the resulting file descriptor, to
     create a second file descriptor.
   * Displays the file offset and the state of the O_APPEND file
     status flag via the first file descriptor.
   * Changes the file offset and setting of the O_APPEND file status
     flag via the first file descriptor.
   * Displays the file offset and the state of the O_APPEND file
     status flag via the second file descriptor.
*/
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/kcmp.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

static void
printFileDescriptionInfo(int fd)
{
    off_t offset = lseek(fd, 0, SEEK_CUR);
    if (offset == -1)
        errExit("lseek");
    int o_append = fcntl(fd, F_GETFL) & O_APPEND;
    if (o_append == -1)
        errExit("fcntl F_GETFL");
    printf("fd %d: offset: %jd, O_APPEND: %s\n", fd, offset, o_append ? "true" : "false");
}

// from kcmp(2):
static int
kcmp(pid_t pid1, pid_t pid2, int type,
        unsigned long idx1, unsigned long idx2)
{
    return syscall(SYS_kcmp, pid1, pid2, type, idx1, idx2);
}

int
main(int argc, char *argv[])
{
    int fd1, fd2, flags, fdcmp;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pathname\n", argv[0]);

    fd1 = open(argv[1], O_RDWR);
    if (fd1 == -1)
        errExit("opening file %s", argv[1]);

    fd2 = dup(fd1);

    if (fd2 == -1)
        errExit("dup2");

    printFileDescriptionInfo(fd1);

    if (lseek(fd1, 0, SEEK_END) == -1)
        errExit("lseek");
    flags = fcntl(fd1, F_SETFL, O_APPEND);
    if (flags == -1)
        errExit("fcntl F_SETFL");

    printFileDescriptionInfo(fd2);

    fdcmp = kcmp(getpid(), getpid(), KCMP_FILE, fd1, fd2);
    if (fdcmp == -1) errExit("kcmp");

    if (fdcmp == 0) {
        printf("fd %d and fd %d refer to the same open file description\n", fd1, fd2);
    } else {
        printf("fd %d and fd %d refer to different open file descriptions\n", fd1, fd2);
    }

    exit(EXIT_SUCCESS);
}
