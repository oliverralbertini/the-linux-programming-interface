/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* main.c */

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
    void *handle;
    void (*funcp)(void);

    handle  = dlopen("./libfoo.so", RTLD_LAZY);
    if (handle == NULL) {
        fprintf(stderr, "dlopen: %s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    dlerror();      /* Clear any outstanding error */

    funcp = dlsym(handle, "foo");

    char *err = dlerror();

    printf("Results after dlsym(): funcp = %p; dlerror() = %s\n",
            (void *) funcp, err);

    if (funcp == NULL && err == NULL)
        printf("\tNote that the return value of dlsym() is NULL, but the "
                "return value of\n"
                "\tdlerror() indicates that there was no error.\n");

    exit(EXIT_SUCCESS);
}
