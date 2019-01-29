/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* fd_sharing.c

   Demonstrate that duplicate file descriptors share the file
   offset and file status flags by writing a program that:

   * Opens a file and duplicates the resulting file descriptor, to
     create a second file descriptor.
   * Displays the file offset and the state of the O_APPEND file
     status flag via the first file descriptor.
   * Changes the file offset and setting of the O_APPEND file status
     flag via the first file descriptor.
   * Displays the file offset and the state of the O_APPEND file
     status flag via the second file descriptor.
*/
#include <fcntl.h>
#include "tlpi_hdr.h"

static void
printFileDescriptionInfo(int fd)
{

    /* FIXME: Display file offset and O_APPEND setting for 'fd' */

}

int
main(int argc, char *argv[])
{
    int fd1, fd2, flags;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pathname\n", argv[0]);

    fd1 = open(argv[1], O_RDWR);
    if (fd1 == -1)
        errExit("opening file %s", argv[1]);

    /* FIXME: Complete as described in comments above */

    exit(EXIT_SUCCESS);
}
