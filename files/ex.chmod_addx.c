/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* chmod_addx.c

   Write a program with the following command-line syntax:

        chmod_addx pathname

   The program should enable execute permission for all classes
   of user (user, group, other) for the specified file, but leave
   the other permissions unmodified.
*/
#include <sys/types.h>
#include <sys/stat.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    struct stat sb;
    char *pathname;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pathname\n", argv[0]);

    pathname = argv[1];

    /* FIXME: Enable execute permission for user, group, and other
       on 'pathname' */

    exit(EXIT_SUCCESS);
}
