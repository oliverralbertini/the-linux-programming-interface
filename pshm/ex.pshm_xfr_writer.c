/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/*  pshm_xfr_writer.c

   Read buffers of data data from standard input into a POSIX shared
   memory object from which it is copied by pshm_xfr_reader.c

   We use a pair of unnamed POSIX semaphores to ensure that the writer
   and reader have exclusive, alternating access to the shared memory
   (i.e., the writer writes a block of text, then the reader reads,
   then the writer writes etc.).  This ensures that each block of
   data is processed in turn by the writer and reader.

   This program needs to be started before the reader process as it
   creates the shared memory and semaphores used by both processes.

   Together, these two programs can be used to transfer a stream of
   data through shared memory as follows:

        $ pshm_xfr_writer < infile &
        $ pshm_xfr_reader > out_file
*/
#include "pshm_xfr.h"

int
main(int argc, char *argv[])
{

    /* FIXME: Declare required variables */

    shm_unlink(SHM_PATH);       /* Delete old instance, if any */

    /* FIXME: Create and size shared memory; size it according to the structure
       to be placed in memory; map the object at an address chosen by system */

    /* FIXME: Initialize reader and writer semaphores; initialize so that
       writer has first access to shared memory. */

    /* FIXME: Transfer blocks of data from stdin to shared memory */

    /* Unlink the shared memory object. Even if the reader is
       still using the object, this is okay. The object will
       be removed only after all open references are closed. */

    shm_unlink(SHM_PATH);

    exit(EXIT_SUCCESS);
}
