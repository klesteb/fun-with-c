
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 1996 by Charles A. Measday                                 */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include "str_priv.h"

/*----------------------------------------------------------------------*/

size_t  str_insert (

#    if __STDC__
        const  char  *substring,
        int  subLength,
        size_t  offset,
        char  *string,
        size_t  maxLength)
#    else
        substring, subLength, offset, string, maxLength)

        char  *substring ;
        int  subLength ;
        size_t  offset ;
        char  *string ;
        size_t  maxLength ;
#    endif

{
/*
 * Function: str_insert.c
 * Version : 1.0
 * Created : 02-Oct-1997
 * Author  : Alex Measday
 *
 * Description
 *
 *    Function str_insert() inserts N characters of text at any position in
 *    a string.
 *
 *    Invocation:
 *
 *        numInserted = str_insert(substring, length, offset, string, maxLength);
 *
 *    where
 *
 *        <substring>         - I
 *            Points to the substring that will be inserted in STRING.  If
 *            this argument is NULL, then LENGTH blanks will be inserted in
 *            STRING.
 *
 *        <length>            - I
 *            Is the length of SUBSTRING.  If LENGTH is less than zero, the
 *            length is determined by searching for the null terminator in
 *            SUBSTRING.
 *
 *        <offset>            - I
 *            Is the character offset (0..N-1) in STRING at which SUBSTRING
 *            will be inserted.
 *
 *        <string>            - I/O
 *            Points to the string into which text will be inserted.
 *
 *        <maxLength>         - I
 *            Is the size of the STRING buffer.  Text that would be shifted
 *            beyond the end of STRING is truncated.
 *
 *        <numInserted>       - O
 *            Returns the number of characters actually inserted.  Normally,
 *            this will just be the length of SUBSTRING.  If, however, the
 *            size of the STRING buffer is insufficient to accomodate the
 *            full shift required for the insertion, NUM_INSERTED will be
 *            less than the length of SUBSTRING.
 *
 * Modification History
 *
 * Variables Used
 */

    char  *s;
    int  length;

/*
 * Main part of function.
 */

    /* Make sure arguments are all valid.                               */

    if (string == NULL)  return(0);
    if ((substring != NULL) && (subLength < 0)) subLength = strlen(substring);
    if (subLength == 0)  return(0);

    /* Compute the number of characters following STRING[OFFSET] that   */
    /* can be shifted right to make room for SUBSTRING.  Stored in      */
    /* variable LENGTH, the number computed includes the null terminator*/
    /* at the end of the string (or an extraneous character if          */
    /* truncation will occur).                                          */

    length = offset + subLength + strlen(&string[offset]) + 1;
    if (length > (int) maxLength)  length = maxLength;
    length = length - subLength - offset;

    /* If there is room enough in the string buffer for the substring   */
    /* to be inserted, then insert it.  Text following STRING[OFFSET]   */
    /* may be truncated, if necessary.                                  */

    if (length > 0) {             /* Shift text N columns to the right. */

        for (s = &string[offset+length-1];  length-- > 0;  s--) {

            s[subLength] = *s;

        }

        s = s + subLength ;

    } else {

        /* If there is insufficient room in the string buffer to insert */ 
        /* the full text of the substring, then insert whatever will fit*/
        /* from the substring. The original text following              */
        /* STRING[OFFSET] will be lost.                                 */

        subLength = subLength + length - 1;
        s = &string[offset+subLength-1];

    }

    /* Copy the substring into the string. Variable S points to the end */
    /* of the room made for inserting the substring.  For example, if   */
    /* the substring will be copied into character positions 2-7 of the */
    /* target string, then S points to character position 7. Variable   */
    /* SUB_LENGTH specifies the number of characters to copy from       */
    /* SUBSTRING.                                                       */

    length = subLength;

    if (substring == NULL) {    /* Insert N blanks? */

        while (length-- > 0)
            *s-- = ' ';

    } else {                /* Insert N characters of text? */

        while (length-- > 0)
            *s-- = substring[length];

    }

    /* Ensure null-termination in case of truncation.                   */

    string[maxLength-1] = '\0';    

    /* This function was extremely tedious to write. The next time you  */
    /* extol the virtues of C, remember that this function would have   */
    /* been a one-liner in FORTRAN 77 or BASIC. (True Alex, but in      */
    /* BASIC or FORTRAN 77 you don't see the underlining code that      */
    /* implements that one line. Kevin)                                 */

    return(subLength);

}

