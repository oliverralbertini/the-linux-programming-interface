/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* setxattr.c

   Set an extended attribute from the command line.
*/
#include <sys/xattr.h>
#include "tlpi_hdr.h"

static void
usageError(char *pname)
{
    fprintf(stderr, "Usage: %s [-cr] pathname name value\n", pname);
    fprintf(stderr, "\t-c  Pure create; "
            "error if name already exists (XATTR_CREATE)\n");
    fprintf(stderr, "\t-r  Pure replace; "
            "error if name does not already exist (XATTR_REPLACE)\n");
    exit(EXIT_FAILURE);
}

int
main(int argc, char *argv[])
{
    int flags, opt;

    flags = 0;
    while ((opt = getopt(argc, argv, "cr")) != -1) {
        switch (opt) {
        case 'c':  flags |= XATTR_CREATE;       break;
        case 'r':  flags |= XATTR_REPLACE;      break;
        case '?': usageError(argv[0]);
        }
    }

    if (argc != optind + 3)
        usageError(argv[0]);

    if (setxattr(argv[optind], argv[optind + 1], argv[optind + 2],
                strlen(argv[optind + 2]), flags) == -1)
        errExit("setxattr");

    exit(EXIT_SUCCESS);
}
