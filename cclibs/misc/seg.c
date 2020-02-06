
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

extern char *xmalloc(int);

/*----------------------------------------------------------------------*/

char *seg(const char *string, int start, int finish) {
/*
 * Function: seg.c
 * Version : v1.0
 * Created : 17-Sep-1998
 * Author  : Kevin Esteb
 *
 * Synposis
 *
 *  The buffer that is returned by this function needs to be freed with 
 *  free(). The following text is taken from the DEC Basic reference manual.
 * 
 *  The SEG$ function extracts a substring from a main string, 
 *  leaving the original string unchanged. 
 *
 *  Format 
 *
 *   str-var = SEG$(str-exp, int-exp1, int-exp2) 
 *
 *  Syntax Rules 
 *
 *   None 
 *
 *  Remarks 
 *
 *   1.  BASIC extracts the substring from str-exp, the main 
 *       string, and stores the substring in str-var. The substring 
 *       begins with the character in the position specified by int- 
 *       exp1 and ends with the character in the position specified 
 *       by  int-exp2. 
 *
 *   2.  If int-exp1 is less than 1, BASIC assumes a value of 1. 
 *
 *   3.  If int-exp1 is greater than int-exp2 or the length of 
 *       str-exp, the SEG$ function returns a null string. 
 *
 *   4.  If int-exp1 equals int-exp2, the SEG$ function returns the 
 *       character at the position specified by int-exp1. 
 *
 *   5.  Unless int-exp2 is greater than the length of str-exp, the 
 *       length of the returned substring equals int-exp2 minus 
 *       int-exp1 plus 1. If int-exp2 is greater than the length of 
 *       str-exp, the SEG$ function returns all characters from 
 *       the position specified by int-exp1 to the end of str-exp. 
 *
 *   6.  If you specify a floating-point expression for int-exp1 or 
 *       int-exp2, BASIC truncates it to LONG integer. 
 *
 * Modifications
 *
 * Variables
 */
 
    char *buff = NULL;
    int length, x, y;
   
/*
 * Main part of function.
 */

    length = strlen(string);
    
    if (start < 0) return(NULL);
   
    if ((buff = xmalloc((finish - start) + 1)) == NULL) return(NULL);
    
       /* if an attempt is made to copy beyond end of string then adjust*/
   
       if (finish > length) {
        
        strcpy(buff, &string[start]);
        
    } else {
        
        for (x = start, y = 0; x < finish; x++, y++) {
            
            buff[y] = string[x];
            
        }
        
    }
        
    return(buff);
    
}

