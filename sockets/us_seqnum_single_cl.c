/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* us_seqnum_single_cl.c

   A simple UNIX stream socket client. Request a sequence number from
   a server.

   Usage: us_seqnum_single_cl server-path

   See also us_seqnum_single_sv.c.
*/
#include <sys/un.h>
#include <sys/socket.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    int seqNum;
    int sfd;
    struct sockaddr_un addr;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s server-path\n", argv[0]);

    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1)
        errExit("socket");

    /* Construct server socket address, and make connection */

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, argv[1], sizeof(addr.sun_path) - 1);

    if (connect(sfd, (struct sockaddr *) &addr,
                sizeof(struct sockaddr_un)) == -1)
        errExit("connect");

    /* Read sequence number from server */

    if (read(sfd, &seqNum, sizeof(int)) != sizeof(int))
        fatal("Error on read");

    printf("Got sequence number: %d\n", seqNum);

    exit(EXIT_SUCCESS);
}
