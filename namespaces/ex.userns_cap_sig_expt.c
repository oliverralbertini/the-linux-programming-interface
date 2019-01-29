/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* userns_cap_sig_expt.c

   Usage: userns_cap_sig_expt "[UID map record]" grandchild-UID

        Example: userns_cap_sig_expt "5 1000 10" 6

   This program creates a child (via clone(CLONE_NEWUSER)) and grandchild
   process (via fork()) that reside in a new user namespace. The first
   command-line argument is the 3-tuple (ID-inside-NS ID-outside-NS length)
   that defines the UID map for the new user namespace. (The GID map just
   maps the process's GID to the same value in the namespace.)

   The second command-line argument is a UID (relative to the new
   user namespace) that the grandchild process should adopt,
   via setresuid().

   The idea is that different users in the initial user namespace could
   try sending signals to the child and granchild process. To do this
   requires either a UID match (following the rules described in kill(2))
   or that the sender has CAP_KILL in the target user namespace.

   Install this program with capabilities:

      setcap cap_setuid,cap_setgid,cap_dac_override=pe userns_cap_sig_expt

   This allows the program to:

   (a) Set up a UID record that has a length value that is greater than 1.
   (b) Write to map files that are owned by root. (The /proc/PID files for
       the processes created by this program will be owned by root:root,
       for the reasons described in the proc(5) manual page.)
*/
#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <sys/capability.h>

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                        } while (0)

int pfd[2];

static void
display_creds(char *str)
{
    printf("%sUID: r=%ld; e=%ld; GID: r=%ld; e=%ld;  ", str,
            (long) getuid(), (long) geteuid(),
            (long) getgid(), (long) getegid());

#if 0      /* Optional code to show process capabilities */
    cap_t caps;
    char *s;

    caps = cap_get_proc();
    if (caps == NULL)
        errExit("cap_get_proc");
    s = cap_to_text(caps, NULL);
    if (s == NULL)
        errExit("cap_to_text");

    printf("capabilities: %s\n", s);
    cap_free(caps);
    cap_free(s);
#else
    printf("\n");
#endif
}

#define STACK_SIZE (1024 * 1024)

static int              /* Start function for cloned child */
childFunc(void *arg)    /* 'arg' is taken from argv[2] */
{
    pid_t grandchild;   /* Grandchild PID returned by fork() */
    char ch;

    /* Wait until the parent has updated the UID and GID mappings.
       See the comment in main(). We wait for end of file on a
       pipe that will be closed by the parent process once it has
       updated the mappings. */

    if (close(pfd[1]) == -1)
        errExit("close");

    if (read(pfd[0], &ch, 1) != 0) {
        fprintf(stderr,
                "Failure in child: read from pipe returned != 0\n");
        exit(EXIT_FAILURE);
    }

    if (close(pfd[0]) == -1)
        errExit("close");

    printf("\nChild PID %ld\n", (long) getpid());

    display_creds("\tchild ");

    /* FIXME: Use fork() to create a grandchild process in the same namespace.
       The grandchild uses setresuid() to set all of its UIDs to the
       value named in the second command-line argument, and then displays
       its credentials with display_creds(). Before terminating, the child
       should sleep() for a while, to allow us to inspect the state of the
       processes, and try sending signals to them. */

    wait(NULL);         /* Wait for grandchild to terminate */

    exit(EXIT_SUCCESS);
}

int
main(int argc, char *argv[])
{
    pid_t child_pid;            /* Child PID returned by clone() */
    char map_path[PATH_MAX];
    char gid_map[1000];
    char *child_stack;
    int fd;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s \"UID map\" UID\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("Parent PID %ld\n", (long) getpid());

    display_creds("\tparent ");

    /* We use a pipe to synchronize the parent and child, in order to
       ensure that the parent sets the UID and GID maps before the
       grandchild makes calls that change process credentials */

    if (pipe(pfd) == -1)
        errExit("pipe");

    /* FIXME: Create a child process in a new user namespace.  argv[2] should
       be passed as the argument of the start function for the child. */

    /* FIXME: Update the UID map file for the new user namespace */

    /* Update the GID map file for the new user namespace.  Note that
       because this program should be set up with the CAP_SETGID
       capability, it is not necessary to update the /proc/PID/setgroups
       file in the new namespace before updating the GID map. */

    snprintf(gid_map, sizeof(gid_map), "%ld %ld 1",
                (long) getegid(), (long) getegid());

    snprintf(map_path, PATH_MAX, "/proc/%ld/gid_map", (long) child_pid);
    fd = open(map_path, O_RDWR);
    if (fd == -1)
        errExit("open-gid_map");

    if (write(fd, gid_map, strlen(gid_map)) != strlen(gid_map))
        errExit("write-gid_map");

    if (close(fd) == -1)
        errExit("close");

    /* Close the write end of the pipe, to indicate to the child
       process that the map files have been updated */

    if (close(pfd[1]) == -1)
        errExit("close");

    if (wait(NULL) == -1)               /* Wait for child */
        errExit("wait");

    exit(EXIT_SUCCESS);
}
