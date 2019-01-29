/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* simple_stat.c

   A program that displays some of the information returned by stat().

   Usage: simple_stat file

   See also t_stat.c
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    struct stat sb;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s file\n", argv[0]);

    if (stat(argv[1], &sb) == -1)
        errExit("stat");

    printf("File type:   ");

    switch (sb.st_mode & S_IFMT) {  /* Extract file type */
    case S_IFREG:  printf("regular file\n");      break;
    case S_IFDIR:  printf("directory\n");         break;
    default:       printf("<other>\n");           break;
    }

    printf("I-node #:   %ld\n", (long) sb.st_ino);

    printf("Mode:       %lo\n",
            (unsigned long) sb.st_mode & 07777);

    printf("# of links: %ld\n", (long) sb.st_nlink);

    printf("Ownership:  UID=%ld   GID=%ld\n",
            (long) sb.st_uid, (long) sb.st_gid);

    printf("File size:  %lld bytes\n", (long long) sb.st_size);

    printf("Last file modification: %s", ctime(&sb.st_mtime));

    exit(EXIT_SUCCESS);
}
