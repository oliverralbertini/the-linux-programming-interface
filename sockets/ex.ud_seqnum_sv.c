/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* ud_seqnum_sv.c

   A simple UNIX datagram socket server. Its service is to provide
   unique sequence numbers to clients.

   Usage: ud_seqnum_sv server-path [init-seq-num]
                                   (default = 0)

   See also ud_seqnum_cl.c.
*/
#include <sys/un.h>
#include <sys/socket.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    struct sockaddr_un addr;
    int sfd;
    int seqNum;

    /* FIXME: Further variable declarations */

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s server-path [init-seq-num]\n", argv[0]);

    if (strlen(argv[1]) > sizeof(addr.sun_path) - 1)
        fatal("socket pathname is too long");

    seqNum = (argc > 2) ? atoi(argv[2]) : 0;

    sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sfd == -1)
        errExit("socket");

    /* FIXME: Construct server socket address and bind socket to it */

    for (;;) {     /* Handle clients iteratively */

        /* FIXME: Read client request containing requested run length */

        /* FIXME: Send response to client and update sequence number */

    }
}
