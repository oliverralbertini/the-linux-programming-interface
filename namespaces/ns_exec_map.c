/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* ns_exec_map.c

   A proof of concept program to answer a question: suppose that we
   use clone(CLONE_NEWUSER) to create a child process in a new user
   namespace, pin the namespace into existence with a bind mount,
   and then have the child process terminate, so that the user namespaces
   is now empty (this scenario could be set up with some assistance from
   the unshare.c program, for example). Can another process then setns()
   into the user namespaces and update the UID and GID maps? This program
   shows that this can be done.
*/
#define _GNU_SOURCE
#include <fcntl.h>
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include "tlpi_hdr.h"

/* Update the mapping file 'map_file', creating a root mapping
   ("0 <id> 1") for the specified 'id'. */

static void
update_self_map(id_t id, char *map_file)
{
    int fd;
    const int MAP_BUF_SIZE = 100;
    char map_buf[MAP_BUF_SIZE];

    snprintf(map_buf, MAP_BUF_SIZE, "0 %ld 1", (long) id);

    fd = open(map_file, O_RDWR);
    if (fd == -1)
        errExit("open %s", map_file);

    if (write(fd, map_buf, strlen(map_buf)) != strlen(map_buf))
        fatal("write: %s : %s", map_file, map_buf);

    close(fd);
}

/* Update /proc/self/setgroups with the string provided in 'str' */

static void
proc_self_setgroups_write(char *str)
{
    int fd;

    fd = open("/proc/self/setgroups", O_RDWR);
    if (fd == -1) {

        /* We may be on an old kernel that doesn't have the file
           /proc/PID/setgroups.  That's fine: in that case, we don't need
           to do anything in order to permit 'gid_map' to be updated.

           However, if the error from open() was something other than the
           ENOENT error that is expected for that case, let the user know. */

        if (errno == ENOENT)
            return;
        else
            errExit("open setgroups");
    }

    if (write(fd, str, strlen(str)) != strlen(str))
        fatal("write setgroups");

    close(fd);
}

int
main(int argc, char *argv[])
{
    uid_t euid;
    uid_t egid;
    int fd;

    if (argc < 3) {
        fprintf(stderr, "%s userns-file cmd [arg...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Save credentials before calling setns(). We do this now, because
       after the setns(), and before we create the UID and GID maps,
       geteuid() and getegid() would return the 'nobody' ID.) */

    euid = geteuid();
    egid = getegid();

    fd = open(argv[1], O_RDONLY);   /* Get descriptor for user namespace */
    if (fd == -1)
        errExit("open");

    if (setns(fd, CLONE_NEWUSER) == -1)         /* Join the user namespace */
        errExit("setns");

    /* Update the uid_map and gid_map files */

    update_self_map(euid, "/proc/self/uid_map");

    proc_self_setgroups_write("deny");
    update_self_map(egid, "/proc/self/gid_map");

    execvp(argv[2], &argv[2]);      /* Execute a command in namespace */
    errExit("execvp");
}
