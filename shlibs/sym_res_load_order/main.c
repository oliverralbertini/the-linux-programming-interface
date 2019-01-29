/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* main.c
*/
#define _GNU_SOURCE
#include <link.h>
#include <stdio.h>
#include <stdlib.h>

static int
callback(struct dl_phdr_info *info, size_t size, void *data)
{
    printf("\tName = %s\n", info->dlpi_name);

    return 0;
}

int
main(int argc, char *argv[])
{
    void func_x1(void);
    void func_y1(void);
    void func_z1(void);

    printf("Link map as shown from dl_iterate_phdr() callbacks:\n");
    dl_iterate_phdr(callback, NULL);
    printf("\n");

    //func_x1();
    //func_y1();
    func_z1();

    exit(EXIT_SUCCESS);
}
