/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* make_link.c

   Usage: <program-name> target linkpath

   An example of a program that varies its behavior based on the value
   in argv[0]. After building this program, create two links to the
   executable, 'hlink' and 'slink':

        ln make_link hlink
        ln make_link slink

   When invoked with the name 'slink', the program creates a symbolic
   link based on its arguments; otherwise it creates a hard link.
*/
#include <libgen.h>     /* For declaration of basename() */
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    char *target, *linkpath;

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s target new-link-path\n", argv[0]);

    target = argv[1];
    linkpath = argv[2];

    /* FIXME: Create a symbolic link or a hard link, depending on whether
       the program is invoked via the name 'slink'. */

    exit(EXIT_SUCCESS);
}
