
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

#include <stdio.h>
#include <string.h>

/*----------------------------------------------------------------------*/

extern char *strupr(char *);
extern char *xmalloc(int);

/*----------------------------------------------------------------------*/

char *fmtasc(char *in_str, char *fmt_str) {
/*
 * Function: fmtasc.c
 * Version : v1.0
 * Created : 21-Dec-1992
 * Author  : Kevin Esteb
 *
 * Synopsis
 *
 *    This function will format a ascii string using a template of what the
 *    output string should look like.
 *
 *    Usage:
 *        outbuf = fmtasc("123456", "xx,xx,xx");
 *
 *    This will format outbuf to look like this "12,34,56". Outbuf will
 *    need to be freed with free().
 *
 * Modification History
 *
 * Variables Used
 */

    char *in  = in_str;
    char *fmt = fmt_str;
    char *out;
	
/*
 * Main part of funtion.
 */

    out = xmalloc(strlen(fmt_str) + 1);
	
    strupr(fmt_str);                    /* Change to upper case             */

    while (*fmt) {

        if (*fmt == 'X') {

            if ((*in >= 32)  &&
                (*in <= 126) &&
                (*in != '\0')) {

                *out = *in;
                *in++;

            } else *out = ' ';

        } else {

            if (*in != '\0')

                *out = *fmt;

            else *out = ' ';

        }

        *out++;
        *fmt++;

    }

    *out = '\0';

	return(out);
	
}

