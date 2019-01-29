/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* pmsg_create_simple.c

   Create a POSIX message queue.  This program is a simplified version
   of pmsg_create.c.
*/
#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    mqd_t mqd;
    struct mq_attr attr;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s /mq-name [max-msg [msg-size]]\n", argv[0]);

    attr.mq_maxmsg  = (argc > 2) ? atoi(argv[2]) : 5;
    attr.mq_msgsize = (argc > 3) ? atoi(argv[3]) : 1024;

    mqd = mq_open(argv[1], O_CREAT | O_EXCL,
                  S_IRUSR | S_IWUSR, &attr);
    if (mqd == (mqd_t) -1)
        errExit("mq_open");

    if (mq_close(mqd) == -1)
        errExit("mq_close");

    exit(EXIT_SUCCESS);
}
