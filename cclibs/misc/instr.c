
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 1996 by Kevin L. Esteb                                     */
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

int instr(int start, const char *buffer, const char *chrs) {
/*
 * Function: instr.c
 * Version : 1.0
 * Author  : Kevin Esteb
 * Date    : 11-Jul-1996
 *
 * Description:
 *
 *  The buffer that is returned by this function needs to be freed with 
 *  free(). The following text is taken from the DEC Basic reference manual.
 * 
 *  INSTR It returns the position of the substring's starting character. 
 *
 *  Format 
 *
 *   int-var = INSTR(int-exp, str-exp1, str-exp2) 
 *
 *  Syntax Rules 
 *
 *   1.   Int-exp specifies the character position in the main string 
 *        at which BASIC starts the search. 
 *
 *   2.   Str-exp1 specifies the main string. 
 *
 *   3.   Str-exp2 specifies the substring. 
 *
 *  Remarks 
 *
 *   1.  The INSTR function searches str-exp1, the main string, 
 *       for the first occurrence of a substring, str-exp2, and 
 *       returns the position of the substring's first character. 
 *
 *   2.  INSTR returns the character position in the main string 
 *       at which BASIC finds the substring, except in the follow- 
 *       ing situations: 
 *
 *        ·   If only the substring is null, and if int-exp is less than 
 *            or equal to zero, INSTR returns a value of 1. 
 *
 *        ·   If only the substring is null, and if int-exp is equal 
 *            to or greater than 1 and less than or equal to the 
 *            length of the main string, INSTR returns the value of 
 *            int-exp  . 
 *
 *        ·   If only the substring is null, and if int-exp is greater 
 *            than the length of the main string, INSTR returns 
 *            the main string's length plus 1. 
 *
 *        ·   If the substring is not null, and if int-exp is greater 
 *            than the length of the main string, INSTR returns a 
 *            value of zero. 
 *
 *        ·   If only the main string is null, INSTR returns a value 
 *            of zero. 
 *
 *        ·   If both the main string and the substring are null, 
 *            INSTR returns a 1. 
 *
 *   3.  If BASIC cannot find the substring, INSTR returns a 
 *       value of zero. 
 *
 *   4.  If int-exp does not equal 1, BASIC still counts from the 
 *       beginning of the main string to calculate the starting po- 
 *       sition of the substring. That is, BASIC counts character 
 *       positions starting at position 1, regardless of where you 
 *       specify the start of the search. For example, if you spec- 
 *       ify 10 as the start of the search and BASIC finds the 
 *       substring at position 15, INSTR returns the value 15. 
 *
 *   5.  If int-exp is less than 1, BASIC assumes a starting position 
 *       of 1. 
 *
 *   6.  If you specify a floating-point expression for int-exp, 
 *       BASIC truncates it to an integer of the default size. 
 *
 * Modification History:
 *
 * Variables:
 */

    int x;
    int buflen;
    int chrlen;

/*
 * Main part of function.
 */

    buflen = strlen(buffer);
    chrlen = strlen(chrs);

    if ((chrlen == 0) && (start < 0)) return(0);
    if ((chrlen == 0) && ((start >= 0) && (start <= buflen))) return(start);
    if ((chrlen == 0) && (start > buflen)) return(buflen + 1);
    if (buflen == 0) return(0);
    if ((chrlen == 0) && (buflen == 0)) return(0);
    if (start > buflen) return(-1);
    if (start < 0) start = 0;

    for (x = start; x < buflen; x++) {

        if (strncmp(&buffer[x], chrs, chrlen) == 0) {

            return(x + 1);

        }

    }

    return(-1);

}

