
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

int pos(const char *buffer, const char *chrs, int start) {
/*
 * Function: pos.c
 * Version : 1.0
 * Author  : Kevin Esteb
 * Date    : 11-Jul-1996
 *
 * Description:
 *
 *    This function emulates the DEC Basic pos() function with the needed
 *    changes for C usage. The rest is taken from the DEC Basic manual.
 *
 *    The POS function searches for a substring within a string and 
 *    returns the substring's starting character position.
 *
 *    Rules:
 *
 *        1.  The POS function searches "buffer", the main string, for the 
 *            first occurance of "chrs", the substring, and returns the 
 *            positon of the first substrings first character.
 *
 *        2.  If "start" is greater then the length of the main string, POS
 *            returns a value of -1.
 *
 *        3.  POS always returns the character positon in the main string at
 *            which this function finds the substring, with the following
 *            exceptions:
 *
 *            o   If only the substring is NULL, and if "start" is less than
 *                or equal to zero, POS returns a value of 0.
 *
 *            o   If only the substring is NULL and if start is equal to or
 *                greater thean 1 and less than or equal to the length of 
 *                the main string, POS returns the value of "start".
 *
 *            o   If only the substring is NULL and if "start" is greater than
 *                the length of the main string, POS returns the main string's
 *                length plus 1.
 *
 *            o   If only the main string is NULL, POS returns a value of zero.
 *
 *            o   If both the main string and the substrings are NULL, POS
 *                returns 0.
 *
 *        4.  If the fuction cannot find the substring, POS returns a value 
 *            of -1.
 *
 *        5.  If "start" is less then 0, this function assumes a starting
 *            positon of 0.
 *
 *        6.  If "start" does not equal 0, this function still counts from the
 *            strings beginning to calculate the starting position of the
 *            substring. That is this function counts characters starting at
 *            positon 0, regardless of where you specify the start of the
 *            search. For example: you specify 10 as the start of the search
 *            and this function finds the substring at position 15, POS 
 *            returns the value of 15.
 *
 *        7.  If you know that the substring is not near the beginning of the
 *            string, specifing a starting position greater then 0 speeds 
 *            the execution of this function.
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

