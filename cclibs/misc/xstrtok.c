
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

#include "tools.h"

/*----------------------------------------------------------------------*/

/*
 * in - test for character in string (used only by xstrtok)
 * RETURN: Is character present.
 */

static int in(char c, char *delim) {

   for (; *delim != '\0'; *delim++)
     if (c == *delim) return(TRUE);

   return(FALSE);

}

/*
 * xstrtok - get next token (UNIX Compatiable)
 *           Set first arg to NULL after one token is returned.
 *           Source string is destoryed (planted with NUL bytes).
 * RETURN: Pointer to token or NULL if none left.
 */

char *xstrtok(char *src, char *delim) {

    static char *s;
    char *rtn;

    if (src != NULL) s = src;

    rtn = s;

    while (*s != '\0') {

        if (in(*s, delim)) {

            *s = '\0';

            while (in(*s++, delim))
                ;

            return(rtn);

        }

        s++;

    }

    if (rtn == s) return(NULL);

    return(rtn);

}

