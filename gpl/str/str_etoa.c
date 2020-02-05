
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

char  *str_etoa (

#    if __STDC__
        char  *string,
        int  length)
#    else
        string, length)

        char  *string ;
        int  length ;
#    endif

{
/*
 * Function: str_etoa.c
 * Version : 1.0
 * Created : 02-Oct-1997
 * Author  : Alex Measday
 *
 * Description
 *
 *    Function str_etoa() converts an EBCDIC string to an ASCII string.  The
 *    conversion table for this program was created using the "dd conv=ascii"
 *    Unix program, so I hope it's right!
 *
 *    Invocation:
 *
 *        asciiString = str_etoa(ebcdicString, length);
 *
 *    where
 *
 *        <ebcdicString>      - I/O
 *            Is a pointer to the EBCDIC string to be converted.  The
 *            EBCDIC-to-ASCII conversion is done in-place, so EBCDIC_STRING
 *            should be in writeable memory.
 *
 *        <length>            - I
 *            Specifies the length of the EBCDIC string.  If LENGTH is
 *            less than zero, str_etoa() determines the length by scanning
 *            EBCDIC_STRING for a terminating null character.
 *
 *        <asciiString>       - O
 *            Returns a pointer to the converted string.  Since the EBCDIC-to-
 *            ASCII conversion is done in-place, this pointer simply points to
 *            the input string, EBCDIC_STRING.
 *
 * Modification History
 *
 * Variables Used
 */

    static  char  ebcdic_to_ascii[256] = {
        0, 1, 2, 3, -100, 9, -122, 127, -105, -115, -114, 11, 12, 13, 14, 15,
        16, 17, 18, 19, -99, -123, 8, -121, 24, 25, -110, -113, 28, 29, 30, 31,
        -128, -127, -126, -125, -124, 10, 23, 27, -120, -119, -118, -117, -116, 5, 6, 7,
        -112, -111, 22, -109, -108, -107, -106, 4, -104, -103, -102, -101, 20, 21, -98, 26,
        32, -96, -95, -94, -93, -92, -91, -90, -89, -88, 91, 46, 60, 40, 43, 33,
        38, -87, -86, -85, -84, -83, -82, -81, -80, -79, 93, 36, 42, 41, 59, 94,
        45, 47, -78, -77, -76, -75, -74, -73, -72, -71, 124, 44, 37, 95, 62, 63,
        -70, -69, -68, -67, -66, -65, -64, -63, -62, 96, 58, 35, 64, 39, 61, 34,
        -61, 97, 98, 99, 100, 101, 102, 103, 104, 105, -60, -59, -58, -57, -56, -55,
        -54, 106, 107, 108, 109, 110, 111, 112, 113, 114, -53, -52, -51, -50, -49, -48,
        -47, 126, 115, 116, 117, 118, 119, 120, 121, 122, -46, -45, -44, -43, -42, -41,
        -40, -39, -38, -37, -36, -35, -34, -33, -32, -31, -30, -29, -28, -27, -26, -25,
        123, 65, 66, 67, 68, 69, 70, 71, 72, 73, -24, -23, -22, -21, -20, -19,
        125, 74, 75, 76, 77, 78, 79, 80, 81, 82, -18, -17, -16, -15, -14, -13,
        92, -97, 83, 84, 85, 86, 87, 88, 89, 90, -12, -11, -10, -9, -8, -7,
        48, 49, 50, 51, 52, 53, 54, 55, 56, 57, -6, -5, -4, -3, -2, -1
    };
    char *s;

/*
 * Main part of function.
 */

    if (length < 0) length = strlen(string);

    for (s = string;  length--;  s++) {

        *s = ebcdic_to_ascii[0x00FF & *s];

    }

    return(string);

}

