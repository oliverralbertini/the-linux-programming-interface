/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* setuid_expt.c

   Enable set-user-ID permission on the executable version of this
   program using the command:

       chmod u+s file
*/
#define _GNU_SOURCE
#include <unistd.h>
#include "tlpi_hdr.h"

static void
displayIds(void)
{
    uid_t ruid, euid, suid;

    if (getresuid(&ruid, &euid, &suid) == -1)
        errExit("getresuid");

    printf("rUID = %ld, eUID = %ld, sUID = %ld\n",
            (long) ruid,  (long) euid,  (long) suid);
}

int
main(int argc, char *argv[])
{
    uid_t euid;

    displayIds();

    euid = geteuid();           /* Obtain copy of privileged UID */

    /* FIXME: Temporarily drop privilege */

    /* FIXME: Regain privilege */

    /* FIXME: Permanently drop privilege */

    /* FIXME: Try once more to regain privilege */

    exit(EXIT_SUCCESS);
}
