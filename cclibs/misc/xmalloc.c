
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
 *     it really did. The returned buffer will be zeroed out. If an error has
 *     occured, NULL will be returned and errno set.
 * 
 * Modification History
 * 
 *    06-Feb-2020 K. Esteb
 *        Removed the reference to vperror.
 * 
 * Variables Used
 * 
 */

    void *p = NULL;

/*
 * Main part of function
 */
    
    errno = 0;
    if ((p = malloc(n)) != NULL) {
        
        memset(p, '\0', n);

    }
   
    return(p);

}

