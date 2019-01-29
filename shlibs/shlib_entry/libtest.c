/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* libtest.c

   By default, executables have an entry point (whose name is "main" by
   default), but shared libraries do not.  However, we can mark a function
   inside the shared library as an entry point using the --entry (-e)
   linker option.  Having done this, we can then execute the shared library
   in the same way that we would an executable, in which case the entry point
   function of the library executed.  This technique is used by a few
   standard libraries, such as glibc. To determine if a shared library
   supports this functionality, you can inspect the shared library using
   something like the following to check whether the library has a ".interp"
   ELF section:

        readelf --section-headers <library> | grep '\.interp'

   Useful background:
   https://stackoverflow.com/questions/31484785/how-to-make-a-linux-shared-object-library-runnable-on-its-own
   */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* When we execute the entry point function of the shared library, the
   library must be dynamically linked in the same manner as an executable.
   When building an executable, the static linker (ld(1)) automatically
   creates an ELF ".interp" section that specifies the standard dynamic
   linker as the program interpreter. However, this does not happen by
   default when we build a shared library. Therefore, we use the
   gcc "__attribute__((section(".interp")))" feature to explicitly specify
   the creation of a ".interp" section that names the dynamic linker. */

#ifndef INTERP

        /* Unless a pathname is provided on the command line, we'll
           assume an x86-64 system and use the standard pathname of
           the dynamic linker on that architecture. */

#define INTERP "/lib64/ld-linux-x86-64.so.2"
#endif

static const char interp[] __attribute__((section(".interp"))) = INTERP;

/* Define an ordinary function that is provided by the library for
   public use */

void
x1(void)
{
    printf("Called function x1\n");
}

/* This is the library "entry point" function */

void
entry_func(void)
{
    printf("Hello world\n");

    /* We must do an explicit exit(), rather than falling off the end
       of the function. In the latter case, we get a segmentation fault
       because there is not the proper C run-time startup code to return
       to (which there is when main() is called in a normal executable). */

    exit(EXIT_SUCCESS);
}
