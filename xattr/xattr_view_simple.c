/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* xattr_view_simple.c

   Display the extended attributes of a file.

   This program is Linux (2.6 and later) specific.
*/
#include <sys/xattr.h>
#include <sys/types.h>
#include "tlpi_hdr.h"

#define XATTR_SIZE 10000

int
main(int argc, char *argv[])
{
    char list[XATTR_SIZE], value[XATTR_SIZE];
    ssize_t ns, llen, vlen;

    if (argc != 2)
        usageErr("%s pathname\n");

    llen = listxattr(argv[1], list, XATTR_SIZE);
    if (llen == -1)
        errExit("listxattr");

    /* Loop through all EA names, displaying name + value */

    for (ns = 0; ns < llen; ns += strlen(&list[ns]) + 1) {
        printf("%s=", &list[ns]);

        vlen = getxattr(argv[1], &list[ns], value, XATTR_SIZE);
        if (vlen == -1)         /* Maybe name was removed */
            printf("couldn't get value\n");
        else            /* Assumes value is printable string */
            printf("%.*s\n", (int) vlen, value);
    }

    exit(EXIT_SUCCESS);
}
