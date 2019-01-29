/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* scm_security_send.c

   Used in conjunction with scm_security_recv.c to demonstrate passing of
   sending socket's SELinux label.

   This program uses stream sockets by default; the "-d" command-line option
   specifies that datagram sockets should be used instead.

   This program is Linux-specific.
*/
#include <selinux/selinux.h>
#include "scm_security.h"

int
main(int argc, char *argv[])
{
    int data, sfd, opt;
    ssize_t numSent;
    struct msghdr msgh;
    struct iovec iov;
    Boolean useDatagramSocket;

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

    char *context;

    if (getcon(&context) == -1)
        errExit("getcon");
    printf("SELinux label = %s\n", context);
    free(context);

    /* The 'msg_name' field can be used to specify the address of the
       destination socket when sending a datagram. However, we do not
       need to use this field because we use connect() below, which sets
       a default outgoing address for datagrams. */

    msgh.msg_name = NULL;
    msgh.msg_namelen = 0;

    /* On Linux, we must transmit at least 1 byte of real data in
       order to send ancillary data */

    msgh.msg_iov = &iov;
    msgh.msg_iovlen = 1;
    iov.iov_base = &data;
    iov.iov_len = sizeof(int);
    data = 12345;

    /* We send no ancillary datat */

    msgh.msg_control = NULL;
    msgh.msg_controllen = 0;

    /* Connect to the peer socket */

    sfd = unixConnect(SOCK_PATH, useDatagramSocket ? SOCK_DGRAM : SOCK_STREAM);
    if (sfd == -1)
        errExit("unixConnect");

    /* Send the data */

    numSent = sendmsg(sfd, &msgh, 0);
    if (numSent == -1)
        errExit("sendmsg");

    printf("sendmsg() returned %ld\n", (long) numSent);

    exit(EXIT_SUCCESS);
}
