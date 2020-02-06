
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

#include <stdlib.h>
#include <string.h>

/*----------------------------------------------------------------------*/

char *strdup(const char *string) {
/*
 * Function: strdup.c
 * Version : 1.0
 * Created : 19-Aug-1999
 * Author  : Kevin Esteb
 * 
 * Description
 * 
 *     This function is for portibility between Borland C++ and GCC.
 * 
 *     Returns a duplicated string.
 * 
 * Modification History
 * 
 * Variables Used
 * 
 */
    
    char *buff = NULL;
    int len;

/*
 * Main part of function.
 */

    len = strlen(string);

    if ((buff = malloc(len + 1)) != NULL) {

        strncpy(buff, string, len);
        buff[len + 1] = '\0';

    }

    return(buff);

}

