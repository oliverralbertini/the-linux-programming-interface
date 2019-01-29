/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* debug_test.c

   A sample program to be used for playing around with gdb.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
    int x;
    int y;
} Vector;

typedef struct bag {
    int pos;
    char *name;
    char marker;
    struct bag *next;
} Bag;

int globint = 0;
static int sglobint = 5;
int xxx = 999;

static void
sub6(void)
{
    printf("We've reached the top\n");
}

static void
sub5(void)
{
    int xxx;            /* Hides global xxx */

    xxx = 5;
}

static void
sub4(int n)
{
    int sub4loc;
    int sub4stat = 0;
    int loc;
    int xxx;            /* Hides global xxx */

    loc = n;
    xxx = 4440 + n;

    sub4stat++;
    sub4loc = n;
    globint++;
    sglobint++;

    if (n > 0)
        sub4(n - 1);
    else
        sub5();
}

static void
sub3(void)
{
#define MAX 10
    int j;
    int a[MAX];
    Vector v;
    Bag b1, b2, b3, b4;
    int sub3loc;
    static int sub3stat = 0;

    sub3stat++;
    globint++;

    sub3loc = 3;

    v.x = v.y = 0;

    for (j = 0; j < MAX; j++) {
        a[j] = j * j;
        v.x += j;
        v.y += a[j];
    }

    b1.pos = 1;
    b1.name = "one";
    b1.marker = '1';
    b1.next = &b2;

    b2.pos = 2;
    b2.name = "two";
    b2.marker = b1.marker;
    b2.next = &b3;

    b3.pos = 3;
    b3.name = "three";
    b3.marker = b1.marker;
    b3.next = &b4;

    b4.pos = 4;
    b4.name = "four";
    b4.marker = b1.marker;
    b4.next = NULL;

    sub4(3);
}

static int
sub2(Vector vec)
{
    int locX, locY;
    Vector v;
    int sub2loc;
    int x;

    x = 2;

    sub2loc = vec.x + vec.y;

    v = vec;
    locX = vec.x;
    locY = vec.y;

    if (vec.x > 0 && vec.y > 0) {
        if (vec.x > vec.y)
            vec.x--;
        else
            vec.y--;
        return sub2(vec);
    } else {
        sub3();
        return vec.x * vec.y;
    }
}

static int
sub1(Vector vec)
{
    int r;
    int sub1loc;
    int x;

    x = 1;

    sub1loc = 1;

    r = sub2(vec);

    return r;
}

int
main(int argc, char *argv[])
{
    Vector v;
    int r;

    v.x = 2;
    v.y = 3;

    r = sub1(v);

    printf("Returned %d\n", r);
    exit(EXIT_SUCCESS);
}
