/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* pmsg_seqnum_sv.c

   A sequence-number server using POSIX message queues.

   See also pmsg_seqnum_cl.c.
*/
#include <mqueue.h>
#include <fcntl.h>

/* FIXME: Include header file defining messages sent from client to server */

#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    char *serverPath;           /* Server's message queue pathname */
    int seqnum;                 /* Sequence number */
    struct mq_attr attr;        /* For mq_open() */
    struct requestMsg req;      /* Message received from client */

    /* FIXME: Further variable declarations */

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s /server-mq-name [init-seqnum]\n", argv[0]);

    serverPath = argv[1];
    seqnum = (argc > 2) ? atoi(argv[2]) : 0;

    mq_unlink(serverPath);      /* In case left over from previous run */

    /* FIXME: Initialize attribute structure needed by mq_open() */

    /* FIXME: Create/open server's message queue for reading */

    /* Read client requests and send responses */

    for (;;) {

        /* FIXME: Receive a client request */

        /* FIXME: Each client has its own message queue for the response. Open
           that queue in nonblocking mode, so that the later mq_send()
           will not block the server in case the client has done
           something silly such as filling its queue with messages. */

        /* FIXME: Send response containing current sequence number to client;
           increment sequence number by "run length" */

        /* FIXME: Close client's message queue */

    }

    exit(EXIT_SUCCESS);
}
