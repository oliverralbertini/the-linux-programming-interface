/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* us_seqnum_cl.c

   A simple UNIX datagram socket client. Request a sequence number from
   a server.

   Usage: ud_seqnum_cl server-path [seq-len]
                                   (default = 1)

   See also ud_seqnum_sv.c.
*/
#include <sys/un.h>
#include <sys/socket.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    int sfd;

    /* FIXME: Further variable declarations */

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s server-path [req-len]\n", argv[0]);

    sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sfd == -1)
        errExit("socket");

    /* FIXME: Bind client's socket to a unique address, so that it can
       receive datagrams sent back by the server */

    /* FIXME: Construct server socket address */

    /* FIXME: Send length of requested sequence to server and receive and
       print server's response */

    /* FIXME: Unlink client socket pathname */

    exit(EXIT_SUCCESS);
}
