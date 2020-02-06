
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 2000 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include "ansi_setup.h"

/*-Prototypes-----------------------------------------------------------*/

int pos(char *, char *, int);
char *left(char *, int);
char *right(char *, int);

/*----------------------------------------------------------------------*/

char *str_replace(char *text, char *old, char *new, char *buffer, int bufsiz) {
/*
 * Function: str_replace.c
 * Version : 1.0
 * Created : 28-Jun-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will replace any occurance of "old" with "new" in 
 *    "text". The result will be placed in "buffer" or returned to the 
 *    calling routine.
 *
 * Modification History
 *
 * Variables Used
 */

    int x;
    int len;
    int done = FALSE;
    char *holder1 = NULL;
	char *holder2 = NULL;

/*
 * Main part of function.
 */

    if (strlen(text) > bufsiz) return(NULL);

    len = strlen(old);
    strcpy(buffer, text);

    while (done == FALSE) {

        if ((x = pos(buffer, old, 1)) > 0) {

            holder1 = left(buffer, x - 1);
            holder2 = right(buffer, x + len);
            strcpy(buffer, holder1);
            strcat(buffer, new);
            strcat(buffer, holder2);

        } else done = TRUE;

    }

    return(buffer);

}

