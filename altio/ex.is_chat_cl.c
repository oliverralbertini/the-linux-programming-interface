/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* is_chat_cl.c

   A simple chat client.

   Usage: is_chat_cl server-host server-port [nickname]

   See also is_chat_sv.c.
*/
#define _GNU_SOURCE
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include "inet_sockets.h"
#include "tlpi_hdr.h"

static int
connectToServer(char *host, char *port)
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sockfd;

    /* Call getaddrinfo() to obtain a list of addresses that
       we can try connecting to */

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_family = AF_UNSPEC;                /* Allows IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;

    if (getaddrinfo(host, port, &hints, &result) != 0)
        errExit("getaddrinfo");

    /* Walk through returned list until we find an address structure
       that can be used to successfully connect a socket */

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sockfd == -1)
            continue;                   /* On error, try next address */

        if (connect(sockfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break;                              /* Success */

        /* Connect failed: close this socket and try next address */

        close(sockfd);
    }

    freeaddrinfo(result);

    if (rp == NULL)
        fatal("Could not connect socket to any address");

    return sockfd;
}

static void
displayMySocketAddress(int sockfd)
{
    struct sockaddr_storage claddr;
    socklen_t addrlen;
    char buf[IS_ADDR_STR_LEN];

    addrlen = sizeof(claddr);
    if (getsockname(sockfd, (struct sockaddr *) &claddr, &addrlen) == -1)
            errExit("getsockname");
    printf("My address is %s\n",
            inetAddressStr((struct sockaddr *) &claddr,
                           addrlen, buf, sizeof(buf)));
}

int
main(int argc, char *argv[])
{
    const int BUF_SIZE = 10000;
    char inputBuf[BUF_SIZE + 1];
    const int MAX_NICKNAME_SIZE = 16;
    char outputBuf[MAX_NICKNAME_SIZE + sizeof ": " + BUF_SIZE + 1];
    struct epoll_event ev, rev;
    char *host, *port, *nickname;       /* Command-line arguments */
    int sockfd;
    int epfd;
    int ready;
    int len;
    ssize_t nr;

    if (argc < 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s host port [nick-name]\n", argv[0]);

    host = argv[1];
    port = argv[2];

    nickname = (argc > 3) ? argv[3] :
                    (getlogin() != NULL) ? getlogin() : "unknown";
    if (strlen(nickname) > MAX_NICKNAME_SIZE)
        nickname[MAX_NICKNAME_SIZE] = '\0';

    sockfd = connectToServer(host, port);
    if (sockfd == -1)
        errExit("connectToServer");

    displayMySocketAddress(sockfd);

    snprintf(outputBuf, sizeof(outputBuf), "%s [joined]\n", nickname);
    write(sockfd, outputBuf, strlen(outputBuf));

    /* FIXME: Create an epoll instance */

    /* FIXME: Add the file descriptors for the connected socket and standard
       input to the interest list of the epoll instance, in both cases
       testing if the descriptor is readable */

    /* FIXME: Loop using epoll_wait() to monitor the file descriptors,
       sending data from standard input to the socket and data from the
       socket to standard output. */

    /* FIXME: NOTE!! It will simplify the code logic if you use epoll_wait()
       to fetch just *one* file descriptor at a time. If both file
       descriptors happen to be ready at the same time, then the *next*
       call to epoll_wait() will return information about the other
       descriptor. */

    exit(EXIT_SUCCESS);
}
