/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* pty_expt.c

   A simple program to experiment with the behavior of pseudoterminals.
*/
#include <sys/wait.h>
#include <ctype.h>
#include <termios.h>
#include "pty_fork.h"           /* Declaration of ptyFork() */
#include "tlpi_hdr.h"

#define MAX_SNAME 1000

static FILE *fp;

static void
handler(int sig)
{
    /* UNSAFE: This handler uses non-async-signal-safe functions
       (fprintf(); see Section 21.1.2) */

    if (sig == SIGINT)
        fprintf(fp, "\tChild got SIGINT\n");
    else if (sig == SIGQUIT)
        fprintf(fp, "\tChild got SIGQUIT\n");
    else if (sig == SIGTSTP)
        fprintf(fp, "\tChild got SIGTSTP\n");
    else if (sig == SIGHUP)
        fprintf(fp, "\tChild got SIGHUP\n");
}

static void
child(void)
{
    struct sigaction sa;
    char ch;
    ssize_t s;

    /* Establish handlers for the terminal-generated signals */

    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = handler;
    if (sigaction(SIGINT, &sa, NULL) == -1)     /* ^C */
        errExit("sigaction");
    if (sigaction(SIGQUIT, &sa, NULL) == -1)    /* ^\ */
        errExit("sigaction");
    if (sigaction(SIGTSTP, &sa, NULL) == -1)    /* ^Z */
        errExit("sigaction");
    if (sigaction(SIGHUP, &sa, NULL) == -1)     /* Terminal hangup */
        errExit("sigaction");

    /* Loop performing reads from pty slave, inspecting what happened.
       Each read may be interrupted by a signal handler; we ignore the
       resulting EINTR error that is returned by read(). */

    for (;;) {
        fprintf(fp, "    Child about to read\n");
        s = read(STDIN_FILENO, &ch, 1);

        if (s == -1) {
            if (errno != EINTR)
                fprintf(fp, "\tread() in child failed: %s\n", strerror(errno));
        } else if (s == 0) {
            fprintf(fp, "\tChild saw EOF; terminating\n");
            exit(EXIT_SUCCESS);
        } else {
            fprintf(fp, "\tChild got character: %c (%d)\n",
                    isprint(ch)  ? ch : '?', ch);
        }
    }
}

int
main(int argc, char *argv[])
{
    int masterFd;
    pid_t childPid;

    /* Open a file stream that refers to the terminal on which
       the program is started; the child will use this stream
       to write informational messages. */

    fp = fopen("/dev/tty", "r+");
    if (fp == NULL)
        errExit("fopen");

    /* Create a child process, with parent and child connected
       via a pty pair. The pty slave is set up as the controlling
       terminal for the child, and the child's three standard file
       descriptors are opened on the slave.*/

    childPid = ptyFork(&masterFd, NULL, 0, NULL, NULL);
    if (childPid == -1)
        errExit("ptyFork");

    if (childPid == 0) {        /* Child */
        child();
        exit(EXIT_SUCCESS);
    }

    /* Parent falls through to here. Interpret each command-line
       argument as the decimal ASCII code of a character that is
       to be written to the pty master. */

    usleep(100000);     /* Give child a chance to get signal handlers
                           set up.  This is imperfect (i.e., theoretically
                           racy), but sufficient for the experimenting
                           done with this program. */

    for (int j = 1; j < argc; j++) {
        char ch;

        ch = atoi(argv[j]);
        printf("Parent writing %d\n", ch);
        write(masterFd, &ch, 1);
        usleep(500000);         /* Slow things down so we can more
                                   easily observe what is happening */
    }

    printf("Parent closing master side of pty\n");

    if (close(masterFd) == -1)
        errExit("close");

    wait(NULL);         /* Wait for child */
    exit(EXIT_SUCCESS);
}
