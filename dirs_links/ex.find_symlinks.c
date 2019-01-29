/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* find_symlinks.c

   Write a program that uses nftw() to traverse the directory subtree
   specified in its command line (or the current working directory if
   no argument is supplied):

        find_symlinks [directory]

   The program should not dereference symbolic links (use FTW_PHYS). For
   each symbolic link that the program finds in the directory tree, the
   pathname of the symbolic link should be printed, along with the content
   (readlink()) of the symbolic link.
*/
#define _GNU_SOURCE
#define _XOPEN_SOURCE 600
#include <ftw.h>
#include <limits.h>
#include "tlpi_hdr.h"

static int
visitFile(const char *fpath, const struct stat *sb,
                int tflag, struct FTW *ftwbuf)
{
    ssize_t len;                /* Return value of raedlink() */
    char buf[PATH_MAX];         /* Output buffer for readlink () */

    /* FIXME: If the currently visited file is a symlink, print the
       symlink pathname and target (use readlink()) */

    /* FIXME: Return a value that tells nftw() to continue traversal */

}

int
main(int argc, char *argv[])
{
    if (nftw((argc < 2) ? "." : argv[1], visitFile, 20, FTW_PHYS) == -1)
        errExit("nftw");

    exit(EXIT_SUCCESS);
}
