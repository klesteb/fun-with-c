
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

char *left(const char *string, int number) {
/*
 * Function: left.c
 * Version : v1.0
 * Created : 23-Dec-1992
 * Author  : Kevin Esteb
 *
 * Synposis
 *
 *	The buffer that is returned must be freed with free(). This function 
 *	mimics the DEC Basic left$() function and text is taken from the DEC Basic
 *	reference manual:
 * 
 *  LEFT$ function extracts a specified substring from a 
 *  string's left side, leaving the main string unchanged. 
 *
 *  Format 
 *
 *   str-var = LEFT[$](str-exp, int-exp) 
 *
 *  Syntax Rules 
 *
 *   1.   Int-exp specifies the number of characters to be extracted 
 *        from the left side of str-exp. 
 *
 *   2.  If you specify a floating-point expression for int-exp, 
 *       BASIC truncates it to an integer of the default size. 
 *
 *  Remarks 
 *
 *   1.  The LEFT$ function extracts a substring from the left of 
 *       the specified str-exp and stores it in str-var. 
 *
 *   2.  If int-exp is less than 1, LEFT$ returns a null string. 
 *
 *   3.  If int-exp is greater than the length of str-exp, LEFT$ 
 *       returns the entire string. 
 *
 * Modifications
 *
 * Variables
 */
 
	char *buff = NULL;
	int length;
	
/*
 * Main part of function.
 */

	length = strlen(string);
	
	if (number < 1) return(NULL);
	
	if ((buff = xmalloc(length + 1)) == NULL) return(NULL);
	
	if (number >= length) {
		
		strncpy(buff, string, length);

	} else {
		
		strncpy(buff, string, number);
		
	}
	
	return(buff);
	
	
}

