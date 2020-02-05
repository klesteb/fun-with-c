
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

#include "drs_priv.h"

/*----------------------------------------------------------------------*/

int drs_util_debug = 0;          /* Global debug switch (1/0 = yes/no). */

/*----------------------------------------------------------------------*/

/**/

int  drsCompare(

#    if __STDC__
        const  void  *p1,
        const  void  *p2)
#    else
        p1, p2)

        void  *p1 ;
        void  *p2 ;
#    endif

{
/*
 * Function: drsCompare.c
 * Version : 1.0
 * Created : 7-Dec-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function drsCompare() is called by the QSORT(3) function to compare
 *    two file names.
 *
 *    Invocation:
 *
 *        comparison = drsCompare(p1, p2);
 *
 *    where:
 *
 *        <p1>                - I
 *        <p2>                - I
 *            are (VOID *) pointers to the (CHAR *) pointers that point to
 *            the two file names being compared.
 *
 *        <comparison>        - O
 *            returns -1, 0, or +1 if the first file name is lexicographically
 *            less than, equal to, or greater than the second file name.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    return(strcmp(*((char **) p1), *((char **) p2)));

}

