/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* nftw_dtree.c

   Demonstrate the use of nftw(3). Walk though the directory tree specified
   on the command line (or the current working directory if no directory
   is specified on the command line), displaying an indented hierarchy
   of files in the tree. For each file, display:

      * a letter indicating the file type (using the same letters
        as "ls -l"), as obtained using stat(2);
      * a string indicating the file type, as supplied by nftw(); and
      * the file's i-node number.
      *
   See also nftw_dir_tree.c
*/
#if ! defined(_XOPEN_SOURCE) || _XOPEN_SOURCE < 500
#define _XOPEN_SOURCE 500
#endif
#include <ftw.h>
#include "tlpi_hdr.h"

static int          /* Function called by nftw() */
dirTree(const char *pathname, const struct stat *statbuf,
        int typeflag, struct FTW *ftwb)
{
    if (typeflag != FTW_NS)  /* Was file stat()-able? */
        printf("%7ld  ", (long) statbuf->st_ino);
    else
        printf("         ");

    printf("%s", (typeflag == FTW_D)   ? "D  " :
                 (typeflag == FTW_DNR) ? "DNR" :
                 (typeflag == FTW_DP)  ? "DP " :
                 (typeflag == FTW_F)   ? "F  " :
                 (typeflag == FTW_SL)  ? "SL " :
                 (typeflag == FTW_SLN) ? "SLN" :
                 (typeflag == FTW_NS)  ? "NS " : "  ");

    printf("    %*s", 4 * ftwb->level, ""); /* Indent */
    printf("%s\n",  &pathname[ftwb->base]); /* Print basename */

    return 0;           /* Tell nftw() to continue */
}

int
main(int argc, char *argv[])
{
    if (nftw((argc > optind) ? argv[optind] : ".",
                dirTree, 20, FTW_PHYS) == -1)
        errExit("nftw");
    exit(EXIT_SUCCESS);
}
