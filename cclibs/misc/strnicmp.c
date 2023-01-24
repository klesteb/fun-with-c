
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

#include <ctype.h>

/*----------------------------------------------------------------------*/

int strnicmp(const char *s1, const char *s2, int length) {
/*
 * Function: strnicmp.c
 * Version : 1.0
 * Created : 16-Nov-1998
 * Author  : Kevin Esteb
 * 
 * Description
 * 
 * 	This function is for portibility between Borland C++ and GCC.
 * 
 * 	Compare two strings with case insenistivity for the specified length. The
 * 	results returned are the same as strcmp().
 * 
 * Modification History
 * 
 * Variables Used
 */
	
	int x;

/*
 * Main part of function
 */
	
	for (x = 0; *s1 != '\0', *s2 != '\0', x < length; *s1++, *s2++, x++) {
		
		if (toupper(*s1) == toupper(*s2)) continue;
		if (toupper(*s1) < toupper(*s2)) return(-1);
		if (toupper(*s1) > toupper(*s2)) return(1);
		
	}
	
	return(0);
	
}

