/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* date_math.c

   Usage: date_math [YYYY-MM-DD] operation...

   Given a date in the form YYYY-MM-DD (or today if no date is supplied),
   convert that date into a broken-down time (BDT) and then interpret the
   remaining arguments as operations specifying years, months, or days
   to be added or subtracted from the corresponding fields of the
   BDT. These operations have the form:

        [+-]nnn[ymd]

   That is, a sign, an integer value, and the character 'y' (years),
   'm' (months), or 'd' (days).

   After performing the calculations, use mktime() to normalize the BDT.
   Then use strftime() to convert the BDT to a string of the form YYY-MM-DD
   and display the resulting string.
*/
#include <ctype.h>
#include <time.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    int firstOp;        /* First operand past optional date */
    int j, n;
    time_t t;           /* Calendar time returned by time() */
    struct tm bdt;
    char field;
    size_t len;
    char buf[sizeof("YYYY-MM-DD")];     /* Output buffer for strftime() */

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s [YYYY-MM-DD] {+|-}nnn[ymd]...\n", argv[0]);

    firstOp = 1;

    /* FIXME: If the first argument is a string of the form YYYY-MM-DD that can
       be converted to a BDT, do so (and increment 'firstOp'). Otherwise,
       get the current time and convert it to a BDT. */

    /* Perform operations on BDT fields according to the remaining
       command-line arguments */

    for (j = firstOp; j < argc; j++) {
        len = strlen(argv[j]);
        field =  argv[j][len - 1];
        if (strchr("-+", argv[j][0]) == NULL || strchr("ymd", field) == NULL)
            fatal("Bad operation: %s", argv[j]);

        argv[j][len - 1] = '\0';
        n = atoi(argv[j]);

        /* FIXME: Add 'n' to field of 'bdt' that corresponds to 'field' */

    }

    /* FIXME: Normalize the BDT, convert to a string, and print the result */

    exit(EXIT_SUCCESS);
}
