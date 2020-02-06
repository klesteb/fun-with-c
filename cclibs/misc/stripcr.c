
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 1999 by Kevin L. Esteb                                     */
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

char *stripcr(char *buff) {
/*
 * Function: stripcr.c
 * Version : 1.0
 * Created : 28-Oct-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will strip the carriage return from the end of a string.
 *
 * Modification History
 *
 * Variables Used
 */

    int x;

/*
 * Main part of function.
 */

    for (x = strlen(buff); x != 0; x--) {

        if (buff[x] == '\r') buff[x] = '\0';

    }

    return(buff);
 
}

