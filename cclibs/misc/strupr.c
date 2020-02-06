
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 1998 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include <string.h>
#include <ctype.h>

/*----------------------------------------------------------------------*/

char *strupr(char *string) { 
/*
 * Function: strupr.c
 * Version : 1.0
 * Created : 16-Nov-1998
 * Author  : Kevin Esteb
 * 
 * Description
 * 
 *     This function is for portibility with Borland C++ and GCC.
 * 
 *     Return an upper cased string.
 * 
 * Modification History
 * 
 * Variables Used
 * 
 */

    char *s = NULL;

/*
 * Main part of function.
 */

    for (s = string; *s; ++s) {

        *s = toupper(*s);

    }

    return(string);

}

