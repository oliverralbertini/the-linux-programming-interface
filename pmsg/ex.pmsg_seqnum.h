/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* pmsg_seqnum.h

   Header file used by pmsg_seqnum_sv.c and pmsg_seqnum_cl.c
*/

#define MAX_MQ_PATH 256         /* Maximum size of MQ pathname */

struct requestMsg {             /* Requests sent by clients to server */

    /* FIXME: Define structure of requests sent by clients to server */

};
