
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

extern char *xmalloc(int);

/*----------------------------------------------------------------------*/

char *mid(const char *string, int start, int length) {
/*
 * Function: mid.c
 * Version : v1.0
 * Created : 23-Dec-1992
 * Author  : Kevin Esteb
 *
 * Synposis
 *
 *  The following text is taken from the DEC Basic reference manual. 
 *  The buffer that is returned from this function needs to be freed.
 * 
 *  The MID$ function extracts a specified substring from a 
 *  string expression. 
 *
 *  Format 
 *
 *   str-var = MID[$](str-exp, int-exp1, int-exp2) 
 *
 *  Syntax Rules 
 *
 *   1.   Int-exp1 specifies the position of the substring's first 
 *        character. 
 *
 *   2.   Int-exp2 specifies the length of the substring. 
 *
 *   Remarks 
 *
 *   1.  If int-exp1 is less than 1, BASIC assumes a starting 
 *       character position of 1. 
 *
 *   2.  If int-exp2 is less than or equal to zero, BASIC assumes a 
 *       length of zero. 
 *
 *   3.  If you specify a floating-point expression for int-exp1 or 
 *       int-exp2, BASIC truncates it to a LONG integer. 
 *
 *   5.  MID$ function 
 *
 *        ·   The MID$ function extracts a substring from str-exp 
 *            and stores it in str-var. 
 *
 *        ·   If int-exp1 is greater than the length of str-exp, 
 *            MID$ returns a null string. 
 *
 *        ·   If int-exp2 is greater than the length of str-exp, 
 *            BASIC returns the string that begins at int-exp1 and 
 *            includes all characters remaining in str-exp. 
 *
 *        ·   If int-exp2 is less than or equal to zero, MID$ returns 
 *            a null string. 
 *
 * Modifications
 *
 * Variables
 */
 
    char *buff;
    int string_len;
   
/*
 * Main part of function.
 */

    string_len = strlen(string);
    
    if (start < 0) return(NULL);
   
    if ((buff = xmalloc(length + 1)) == NULL) return(NULL);
    
    start--;                /* Adjust for zero based strings            */

    /* If an attempt is made to copy beyond end of string then adjust.  */
   
    if ((start + length) > string_len) {

        memmove(buff, &string[start], string_len - start);

    } else {

        memmove(buff, &string[start], length);

    }
        
    return(buff);
    
}

