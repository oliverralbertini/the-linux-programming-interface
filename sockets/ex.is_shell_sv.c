/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* is_shell_sv.c

   A simple Internet stream socket server. Our (entirely unsecured!) service
   executes shell commands provided by the client, and sends the output
   back to the client.

   Usage:  is_shell_sv server-port

   See also is_shell_cl.c.
*/
#define _GNU_SOURCE
#include <ctype.h>
#include <netdb.h>
#include <signal.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "ignore_signal.h"
#include "tlpi_hdr.h"

#define BACKLOG 50

/* FIXME: Some functionality needed by main() might be implemented as
   separate functions here */

int
main(int argc, char *argv[])
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;

    /* FIXME: Other variable declarations */

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s server-port\n", argv[0]);

    /* Call getaddrinfo() to obtain a list of addresses that
       we can try binding to */

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    /* FIXME: Initialize ai_socktype, ai_family, and ai_flags in 'hints' */

    if (getaddrinfo(NULL, argv[1], &hints, &result) != 0)
        errExit("getaddrinfo");

    /* Walk through returned list until we find an address structure
       that can be used to successfully create and bind a socket */

    for (rp = result; rp != NULL; rp = rp->ai_next) {

        /* FIXME: For each addrinfo entry, try to create a socket and bind
           to the corresponding socket address */

    }

    freeaddrinfo(result);

    if (rp == NULL)
        fatal("Could not bind socket to any address");

    /* FIXME: Make this a listening socket */

    for (;;) {     /* Handle clients concurrently */

        /* FIXME: Accept a client connection */

        /* FIXME: Create a child process that handles the client and then
           terminates; the parent goes on to accept another connection */

    }
}
