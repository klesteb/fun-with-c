
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 1992 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include <string.h>

/*----------------------------------------------------------------------*/

char *strrev(char *buffer) {
/*
 * Function: strrev.c
 * Version : v1.0
 * Created : 21-Dec-1992
 * Author  : Kevin Esteb
 *
 * Synopsis
 *
 *    Reverse the buffer. For compatibility with Borland C++.
 * 
 * Modification History
 *
 * Variables Used
 */

    int x = (strlen(buffer) - 1);
    int y = 0;
    char a;

/*
 * Main part of funtion.
 */

    for (; y < x; x--, y++) {

        a = buffer[x];
        buffer[x] = buffer[y];
        buffer[y] = a;

    }

    return(buffer);

}

