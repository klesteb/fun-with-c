
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 1998 by Kevin L. Esteb                                     */
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
#include <errno.h>

#include "vperror.h"

/*----------------------------------------------------------------------*/

void *xmalloc(int n) {                  /* number of bytes to allocate  */
/*
 * Function: xmalloc.c
 * Version : 1.0
 * Created : 16-Nov-1998
 * Author  : Kevin Esteb
 * 
 * Description
 * 
 *     This function will alloc the given amount of memory and check to see if
 *     it really did. If it did not then it will call vperror(). The returned 
 *     buffer will be zeroed out.
 * 
 * Modification History
 * 
 * Variables Used
 * 
 */
    
    void *p = NULL;
    
/*
 * Main part of function
 */
    
    if ((p = malloc(n)) == NULL) {
        
        vperror("(xmalloc) - out of memory\n");

    }
    
    memset(p, '\0', n);
   
    return(p);

}

