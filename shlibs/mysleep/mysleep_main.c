/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* mysleep_main.c
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    extern void mysleep(int nsecs);

    if (argc != 2) {
        fprintf(stderr, "Usage: %s num-secs\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("My PID is %ld\n", (long) getpid());

    mysleep(atoi(argv[1]));

    exit(EXIT_SUCCESS);
}
