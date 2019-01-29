/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* seccomp_no_children.c

   Install a seccomp filter that prevents the creation of children
   using fork() or clone().
*/
#define _GNU_SOURCE
#include <stddef.h>
#include <fcntl.h>
#include <linux/audit.h>
#include <sys/syscall.h>
#include <linux/filter.h>
#include <linux/seccomp.h>
#include <sys/prctl.h>
#include "tlpi_hdr.h"

/* For the x32 ABI, all system call numbers have bit 30 set */

#define X32_SYSCALL_BIT         0x40000000

static int
seccomp(unsigned int operation, unsigned int flags, void *args)
{
    return syscall(__NR_seccomp, operation, flags, args);
}

static void
install_filter(void)
{
    struct sock_filter filter[] = {

        /* FIXME: Load the architecture and kill the process if the
           architecture is not what we expect */

        /* FIXME: Load the system call number; kill process if this iscw
           an x32 system call */

        /* FIXME: Make fork() calls fail with ENOTSUP and clone() calls fail
           with EPERM; allow other system calls */

    };

    /* Initialize structure that describes the BPF program */

    struct sock_fprog prog = {
        .len = (unsigned short) (sizeof(filter) / sizeof(filter[0])),
        .filter = filter,
    };

    /* Install the BPF filter */

    if (seccomp(SECCOMP_SET_MODE_FILTER, 0, &prog) == -1)
        errExit("seccomp");
    /* On Linux 3.16 and earlier, we must instead use:
            if (prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &prog))
                errExit("prctl-PR_SET_SECCOMP");
    */
}

int
main(int argc, char **argv)
{
    if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0))
        errExit("prctl");

    install_filter();

    execvp(argv[1], &argv[1]);
    errExit("execvp");
}
