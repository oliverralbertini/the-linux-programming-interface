/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* main_thread_exit.c

*/
#include <pthread.h>
#include "tlpi_hdr.h"

/* FIXME: Implement a thread-start function */

int
main(int argc, char *argv[])
{
    pthread_t thr;
    int s;

/* FIXME: Create a thread that executes the thread-start function */

    printf("Main thread exiting");

/* FIXME: Terminate main thread */

}
