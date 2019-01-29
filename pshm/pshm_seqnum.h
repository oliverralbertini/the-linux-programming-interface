/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* pshm_seqnum.h

   Header file used by pshm_seqnum_int.c and pshm_seqnum_get.c.
*/
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include "tlpi_hdr.h"

struct shmbuf {         /* Shared memory buffer */
    sem_t sem;          /* Semaphore to protect access */
    int seqnum;         /* Sequence number */
};
