/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* born_to_be_roadkill.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int
main(int argc, char *argv[])
{
    char buf[16];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s arg\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    strcpy(buf, argv[1]);
    printf("Successfully copied: %s\n", buf);

    return 0;  /* Ooops! Saved return address has been trashed */
}
