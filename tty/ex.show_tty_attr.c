/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* show_tty_attr.c

   Display some of the termios attributes for the terminal connected
   to standard input.
*/
#include <termios.h>
#include "tlpi_hdr.h"

/* Display a flag name and its setting as "on" or "off" */

static void
displayFlag(const char *name, int setting)
{
    printf("%-10s: %s\n", name, setting ? "on" : "off");
}

int
main(int argc, char *argv[])
{
    struct termios tbuf;

    /* FIXME: Retrieve the termios structure for the terminal referred to by
       standard input */

    /* FIXME: Display settings of various terminal flags */

    /* FIXME: If the terminal is in noncanonical mode, display the TIME
       and MIN values (contained in the c_cc array) */

}
