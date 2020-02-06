
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

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*----------------------------------------------------------------------*/

char *ltrim(char *string) {
/*
 * Function: ltrim.c
 * Version : v1.1
 * Created : 23-Dec-1992
 * Author  : Kevin Esteb
 *
 * Synposis
 *
 *    Removes all of the leftmost spaces from the string specified
 *    by "string".
 *
 * Modifications
 *
 *    18-Jul-2002 K.Esteb
 *        I noticed that the resulting string was not being properly 
 *        terminated, so I fixed that. Then I noticed that the temporary
 *        buffer was not being freed. So I fixed that also.
 *
 * Variables
 */
 
    char *buff = NULL;
    int len;

/*
 * Main part of function.
 */

    if (string != NULL) {
        
        buff = strdup(string);
        
        while (isspace(*buff)) {
            
            *buff++;
            
        } 
        
        len = strlen(buff);
        strncpy(string, buff, len);
        string[len] = '\0';
/*        free(buff); */

    }

    return(string);

}

