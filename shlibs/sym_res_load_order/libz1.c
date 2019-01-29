/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

#include <stdio.h>

void
func_z1(void)
{
    void abc(void);
    void xyz(void);

    printf("Called %s::%s\n", __FILE__,  __FUNCTION__);

    printf("\n%s::%s: about to call abc()\n", __FILE__,  __FUNCTION__);
    abc();

    printf("\n%s::%s: about to call xyz()\n", __FILE__,  __FUNCTION__);
    xyz();
}
