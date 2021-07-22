
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2021 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that this copyright notice appears in all copies. The author    */
/*  makes no representations about the suitability of this software for      */
/*  any purpose. It is provided "as is" without express or implied warranty. */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#include <string.h>

/*----------------------------------------------------------------------*/

int xindex(const char *string, const char ch, int offset) {
/*
 * Function: xindex.c
 * Version : 1.0
 * Author  : Kevin Esteb
 * Date    : 21-Jul-2021
 *
 * Description:
 *
 *  This function will return the position of a character within a buffer.
 *  This is ones based. A -1 is returned if the character is not found.
 * 
 * Modification History:
 *
 * Variables:
 */

    int idx = offset - 1;
    const char *e = string;

/*
 * Main part of function.
 */

    if (idx > strlen(e)) {

        return -1;

    }

    if (idx < 0) {

        idx = 0;

    }

    while (e[idx++]) {

        if (e[idx] == ch) {

            idx = ((idx - offset) + 1);
            break;

        }

    }

    return idx;

}

