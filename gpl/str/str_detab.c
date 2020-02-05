
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

size_t  str_detab (

#    if __STDC__
        char  *stringWithTabs,
        int  length,
        int  tabStops,
        char  *stringWithoutTabs,
        size_t  maxLength)
#    else
        stringWithTabs, length, tabStops, stringWithoutTabs, maxLength)

        char  *stringWithTabs ;
        int  length ;
        int  tabStops ;
        char  *stringWithoutTabs ;
        size_t  maxLength ;
#    endif

{
/*
 * Function: str_detab.c
 * Version : 1.0
 * Created : 02-Oct-1997
 * Author  : Alex Measday
 *
 * Description
 *
 *    Function str_detab() converts tabs in a string to blanks.
 *
 *    Invocation:
 *
 *    detabbedLength = str_detab(stringWithTabs, length, tabStops,
 *                               stringWithoutTabs, maxLength);
 *
 *    where
 *
 *        <stringWithTabs>    - I/O
 *            Is a pointer to the string containing tabs.
 *
 *        <length>            - I
 *            Specifies the length of the string containing tabs.  If LENGTH
 *            is less than zero, strDetab determines the length by scanning
 *            STRING_WITH_TABS for a terminating null character.
 *
 *        <tabStops>          - I
 *            Specifies the number of characters between tab stops.  The
 *            default is 8 characters.
 *
 *        <stringWithoutTabs> - I/O
 *            Is a pointer to a string buffer that will receive the expanded
 *            string.  The string will always be null-terminated (and truncated
 *            to a length of MAX_LENGTH-1 if necessary).  If this argument is
 *            NULL, str_detab() performs the conversion in place on
 *            STRING_WITH_TABS, subject to the MAX_LENGTH restriction.
 *
 *        <maxLength>         - I
 *            Is the size of the STRING_WITHOUT_TABS buffer that will receive
 *            the expanded string.  If the STRING_WITHOUT_TABS pointer is NULL,
 *            then MAX_LENGTH specifies the maximum size of the STRING_WITH_TABS
 *            buffer.
 *
 *        <detabbedLength>    - O
 *            Returns the length of the expanded string.
 *
 * Modification History
 *
 * Variables Used
 */

    char  *s;
    int  i, numSpaces;

/*
 * Main part of function.
 */

    if (stringWithTabs == NULL) {

        if (stringWithoutTabs != NULL) *stringWithoutTabs = '\0';

        return (0);

    }

    if (length < 0)  length = strlen(stringWithTabs);
    if (tabStops <= 0)  tabStops = 8;

    if (stringWithoutTabs == NULL) {

        stringWithoutTabs = stringWithTabs;

    } else str_copy(stringWithTabs, length, stringWithoutTabs, maxLength);

    /* For each tab character in the string, delete the tab character   */
    /* and insert the number of spaces necessary to shift the following */
    /* text to the next tab stop.                                       */

    for (i = 0, s = stringWithoutTabs;  *s != '\0';  i++, s++) {

        if (*s != '\t') continue;

        numSpaces = tabStops - (i % tabStops) - 1;
        *s = ' ';

        if (numSpaces > 0) {

            numSpaces = str_insert(NULL, numSpaces, 0, s, maxLength - i);
            s = s + numSpaces;
            i = i + numSpaces;

        }

    }

    return(str_trim(stringWithoutTabs, -1));

}

