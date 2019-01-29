/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* is_chat_sv.c

   A simple chat server.

   Usage: is_chat_sv server-port
*/
#define _GNU_SOURCE
#include <ctype.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/wait.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "ignore_signal.h"
#include "tlpi_hdr.h"

#define BACKLOG 50

/* Global variables describing connected clients */

static int *clientList = NULL;  /* List of connected clients. Index is by
                                   file descriptor number. Positive value
                                   means slot is in use. Negative value
                                   means slot is free. */
static int clientListSize = 0;  /* Size of array pointed to by 'fdList' */
static int numClients = 0;      /* Count of connected clients */

static int
createListeningSocket(char *port)
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    socklen_t optval;
    int listenfd;

    /* Call getaddrinfo() to obtain a list of addresses that
       we can try binding to */

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_UNSPEC;        /* Allows IPv4 or IPv6 */
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;
                        /* Wildcard IP address; service name is numeric */

    if (getaddrinfo(NULL, port, &hints, &result) != 0)
        errExit("getaddrinfo");

    /* Walk through returned list until we find an address structure
       that can be used to successfully create and bind a socket */

    optval = 1;
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        listenfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (listenfd == -1)
            continue;                   /* On error, try next address */

        if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
                    &optval, sizeof(optval)) == -1)
             errExit("setsockopt");

        if (bind(listenfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;                      /* Success */

        /* bind() failed: close this socket and try next address */

        close(listenfd);
    }

    freeaddrinfo(result);

    if (rp == NULL)
        fatal("Could not bind socket to any address");

    if (listen(listenfd, BACKLOG) == -1)
        errExit("listen");

    return listenfd;
}

/* Change the setting of the O_NONBLOCK flag for the open file
   description referred to by 'fd'. If 'setting' is 0, disable
   O_NONBLOCK; otherwise enable it. */

static void
setNonblockFlag(int fd, int setting)
{
    int flags;

    flags = fcntl(fd, F_GETFL);
    if (flags == -1)
        errExit("fcntl");

    flags = setting ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);

    if (fcntl(fd, F_SETFL, flags) == -1)
        errExit("fcntl");
}

/* Add a file descriptor to an epoll interest list */

static void
epollWatch(int epfd, int fd)
{
    struct epoll_event ev;

    ev.events = EPOLLIN;
    ev.data.fd = fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1)
        errExit("epoll_ctl fd=%d", fd);
}

static void broadcastMessage(int fdSource, char *buf, ssize_t nr);

static void
sanitize(char *buf)
{
    char *p;

    for (p = buf; *p != '\0'; p++) {
        if (!isprint(*p) && !isspace(*p))
            *p = '.';
    }
}

/* Accept a new client connection */

static int
acceptConnection(int listenfd)
{
    struct sockaddr_storage claddr;
    socklen_t addrlen;
#define ADDRSTRLEN (NI_MAXHOST + NI_MAXSERV + 10)
    char addrStr[ADDRSTRLEN];
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    char buf[100 + ADDRSTRLEN];
    int cfd;

    /* Accept a client connection, obtaining client's address */

    addrlen = sizeof(struct sockaddr_storage);
    cfd = accept(listenfd, (struct sockaddr *) &claddr, &addrlen);
    if (cfd == -1) {
        if (errno != EINTR)
            errMsg("accept");
        return -1;
    }

    if (getnameinfo((struct sockaddr *) &claddr, addrlen,
                 host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
               snprintf(addrStr, ADDRSTRLEN, "(%s, %s)", host, service);
    else
        snprintf(addrStr, ADDRSTRLEN, "(?UNKNOWN?)");

    /* It is unlikely, but theoretically possible that the DNS request
       was hijacked and we were returned a dangerous string. To be sure
       sanitize it as we would any other program input. */

    sanitize(addrStr);

    /* FIXME: Broadcast a message to other clients about new connection */

    return cfd;
}

/* Close a client connection */

static void
closeConnection(int fd)
{
    char *msg = "[disconnecting]\n";

    numClients--;

    /* Let other users know about the disconnection */

    broadcastMessage(fd, msg, strlen(msg));

    close(fd);                  /* Automatically removes fd from
                                   epoll interest list */
    clientList[fd] = -1;        /* Make slot available for reuse */
}

/* Broadcast the message in 'buf' (containing 'numBytes' bytes) to all
   clients except the one on file descriptor 'fdSource' */

static void
broadcastMessage(int fdSource, char *buf, ssize_t numBytes)
{

    /* FIXME: To be completed as described in comments above */

}

/* Record details of a new client */

static void
addClient(int fd)
{
    const int FDINCR = 100;
    int j;

    /* If client list is too small, reallocate it */

    if (fd >= clientListSize) {
        clientList = realloc(clientList, (fd + FDINCR) * sizeof(*clientList));
        if (clientList == NULL)
            errExit("realloc");

        for (j = clientListSize; j < fd + FDINCR; j++)
            clientList[j] = -1;

        clientListSize = fd + FDINCR;
    }

    if (clientList[fd] > 0)
        fatal("FD list item %d already in use", fd);

    clientList[fd] = 1;         /* Mark slot as in use */
}

/* FIXME: Implement other functions as required */

int
main(int argc, char *argv[])
{
    int listenfd;
    int epfd;
    int ready;
    struct epoll_event rev;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s server-port\n", argv[0]);

    /* Create a listening socket bound to the port specified
       in the command line */

    listenfd = createListeningSocket(argv[1]);
    if (listenfd == -1)
        errExit("createListeningSocket");

    setNonblockFlag(listenfd, 1);       /* Never block on accept() */

    /* Create an epoll instance and add the listening socket to the
       interest list */

    epfd = epoll_create(20);
    if (epfd == -1)
        errExit("epoll_create");

    epollWatch(epfd, listenfd);

    /* Ignore SIGPIPE signal, so that broken connection
       doesn't kill server during write() */

    if (ignoreSignal(SIGPIPE))
        errExit("ignoreSignal");

    /* FIXME: Loop using epoll_wait() to wait for new client connections
       on the listening socket and input from connected clients */

}
