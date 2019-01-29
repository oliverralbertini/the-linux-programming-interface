/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* scm_security_recv.c

   Used in conjunction with scm_security_send.c to demonstrate passing of
   sending socket's SELinux label.

   This program uses stream sockets by default; the "-d" command-line option
   specifies that datagram sockets should be used instead.

   This program is Linux-specific, and requires a system that supports SELinux.
*/
#define _GNU_SOURCE
#include <limits.h>
#include "scm_security.h"

#define BUF_SIZE 100

#ifndef SCM_SECURITY
#define SCM_SECURITY 0x03
#endif

int
main(int argc, char *argv[])
{
    int data, lfd, sfd, opt, optval;
    ssize_t NumReceived;
    Boolean useDatagramSocket;
    struct msghdr msgh;
    struct iovec iov;

    /* Allocate a char array of suitable size to hold the ancillary data.
       However, since this buffer is in reality a 'struct cmsghdr', use a
       union to ensure that it is aligned as required for that structure.
       Alternatively, we could allocate the buffer using malloc(), which
       returns a buffer that satisfies the strictest alignment
       requirements of any type */

    union {
        char   buf[CMSG_SPACE(NAME_MAX)];
                        /* Space large enough to hold 'NAME_MAX' bytes */
        struct cmsghdr align;
    } controlMsg;
    struct cmsghdr *cmsgp;      /* Pointer used to iterate through
                                   headers in ancillary data */

    /* Parse command-line options */

    useDatagramSocket = FALSE;

    while ((opt = getopt(argc, argv, "d")) != -1) {
        switch (opt) {
        case 'd':
            useDatagramSocket = TRUE;
            break;

        default:
            usageErr("%s [-d]\n"
                     "        -d           use datagram socket\n", argv[0]);
        }
    }

    /* Create socket bound to a well-known address. In the case where
       we are using stream sockets, also make the socket a listening
       socket and accept a connection on the socket. */

    if (remove(SOCK_PATH) == -1 && errno != ENOENT)
        errExit("remove-%s", SOCK_PATH);

    if (useDatagramSocket) {
        sfd = unixBind(SOCK_PATH, SOCK_DGRAM);
        if (sfd == -1)
            errExit("unixBind");

    } else {
        lfd = unixBind(SOCK_PATH, SOCK_STREAM);
        if (lfd == -1)
            errExit("unixBind");

        if (listen(lfd, 5) == -1)
            errExit("listen");

        sfd = accept(lfd, NULL, NULL);
        if (sfd == -1)
            errExit("accept");
    }

    /* The 'msg_name' field can be set to point to a buffer where the
       kernel will place the address of the peer socket. However, we don't
       need the address of the peer, so we set this field to NULL. */

    msgh.msg_name = NULL;
    msgh.msg_namelen = 0;

    /* Set fields of 'msgh' to point to a buffer used to receive
       the (real) data read by recvmsg() */

    msgh.msg_iov = &iov;
    msgh.msg_iovlen = 1;
    iov.iov_base = &data;
    iov.iov_len = sizeof(int);

    /* Set 'msgh' fields to describe the ancillary data buffer */

    msgh.msg_control = controlMsg.buf;
    msgh.msg_controllen = sizeof(controlMsg.buf);

    /* FIXME: Set the SO_PASSSEC socket option, so as to receive
       SCM_SECURITY ancillary message */

    /* FIXME: Receive real plus ancillary data */

    /* FIXME: Set 'cmsgp' to point to first ancillary message header */

    /* FIXME: Check the validity of the 'cmsghdr' pointed to by 'cmsgp' */

    /* FIXME: Display the SELinux label contained in the data portion of
       the 'cmsghdr' structure pointed to by 'cmsgp' */

    exit(EXIT_SUCCESS);
}
