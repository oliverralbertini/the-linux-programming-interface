/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* pshm_xfr.h

   Header file used by pshm_xfr_reader.c and pshm_xfr_writer.c.
*/
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include "tlpi_hdr.h"

#define SHM_PATH "/pshm_xfr"    /* Pathname for shared memory object */

#define BUF_SIZE 1024           /* Size of transfer buffer */

struct shmbuf {                 /* Shared memory buffer */
    sem_t wsem;                 /* Post when it's writer's turn */
    sem_t rsem;                 /* Post when it's reader's turn */
    int cnt;                    /* Number of bytes used in 'buf' */
    char buf[BUF_SIZE];         /* Data being transferred */
};
