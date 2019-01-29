/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* mmap_create.c
*/
#define _GNU_SOURCE
#include <sys/mman.h>
#include <ctype.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    char *addr, *p;
    int flags, j, touch, factor;
    size_t size;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "Usage: %s map-spec...\n\n", argv[0]);
        fprintf(stderr, "Each 'map-spec' has the form 'flags' followed "
                "by a 'size'\n");
        fprintf(stderr, "A mapping of the given 'size' is created\n");
        fprintf(stderr, "'flags' is zero or more of the following:\n");
        fprintf(stderr, "\tl    Use MAP_LOCKED flag\n");
        fprintf(stderr, "\tp    Use MAP_POPULATE flag\n");
        fprintf(stderr, "\tr    Use MAP_NORESERVE flag\n");
        fprintf(stderr, "\tt    Touch each page of mapping after creation\n");
        fprintf(stderr, "\tk    Use kibibytes as unit for 'size'\n");
        fprintf(stderr, "\tm    Use mibibytes as unit for 'size'\n");
        fprintf(stderr, "\tg    Use gibibytes as unit for 'size'\n");
        fprintf(stderr, "'size' can be expressed in decimal, octal, or "
                "hexadecimal\n\n");
        fprintf(stderr, "Example: %s 8192 r020000 rp0x2000\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    for (j = 1; j < argc; j++) {
        p = argv[j];
        flags = MAP_PRIVATE | MAP_ANONYMOUS;
        touch = 0;
        factor = 1;
        while (isalpha((unsigned char) *p)) {
            switch (*p) {
            case 'l':   flags |= MAP_LOCKED;            break;
            case 'p':   flags |= MAP_POPULATE;          break;
            case 'r':   flags |= MAP_NORESERVE;         break;
            case 'k':   factor = 1024;                  break;
            case 'm':   factor = 1024 * 1024;           break;
            case 'g':   factor = 1024 * 1024 * 1024;    break;
            case 't':   touch = 1;                      break;
            default:    fatal("Bad flag '%c' in \"%s\"", *p, argv[j]);
            }
            p++;
        }

        size = strtoul(p, NULL, 0) * factor;

        addr = mmap(NULL, size, PROT_READ | PROT_WRITE, flags, -1, 0);
        if (addr == MAP_FAILED)
            errExit("mmap");
        printf("%d: mapping of size 0x%lx (%ld) created at %p\n",
                j, (long) size, (long) size, (void *) addr);
        if (touch) {
            printf("\tTouching pages of mapping\n");
            for (p = addr; p < addr + size; p += 1024)
                *p = '\0';
        }
    }

    printf("Sleeping for 15 seconds\n");
    sleep(15);

    exit(EXIT_SUCCESS);
}
