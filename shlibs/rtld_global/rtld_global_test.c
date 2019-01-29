/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* rtld_global_test.c

   This program loads two shared libraries, libfirst.so and libsecond.so
   and then calls a function in libsecond.so that tries to call a function
   in libfirst.so. This will suceed only if libfirst.so was opened with
   the RTLD_GLOBAL global, which permits the libraries global symbols to
   be used to resolve symbol references in subsequently loaded libraries.

   By default, the program opens libfirst.so with the RTLD_GLOBAL flag,
   and the function call made from libsecond.so succeeds. If a
   command-line argument is supplied when running this program, then
   libfirst.so without the RTLD_GLOBAL flag, and the function call made
   from libsecond.so fails ("symbol lookup error").

*/
#define _GNU_SOURCE
#include <elf.h>
#include <dlfcn.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int
main(int argc, char *argv[])
{
    void *firstHandle, *secondHandle;
    void (*funcp)(void);
    const char *err;
    int flags;

    flags = RTLD_LAZY;

    /* Open first library. If no command-line arguments were supplied, then
       open the library with RTLD_GLOBAL, so that the symbols in this library
       are available for libraries that are subsequently dynamically loaded. */

    if (argc == 1)
        flags |= RTLD_GLOBAL;

    firstHandle = dlopen("./libfirst.so", flags);
    if (firstHandle == NULL) {
        fprintf(stderr, "dlopen failed: %s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    /* Open second library */

    secondHandle = dlopen("./libsecond.so", flags);
    if (secondHandle == NULL) {
        fprintf(stderr, "dlopen failed: %s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    /* Look up address of 'func2' in libsecond.so */

    (void) dlerror();
    *(void **) (&funcp) = dlsym(secondHandle, "func2");
    err = dlerror();
    if (err != NULL) {
        fprintf(stderr, "dlsym failed: %s\n", err);
        exit(EXIT_FAILURE);
    }

    printf("Successfully looked up \"func2\" in \"libsecond.so\"\n");

    /* Now call 'func2', which will in turn try to call 'func1' */

    (*funcp)();

    exit(EXIT_SUCCESS);
}
