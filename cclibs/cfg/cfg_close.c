
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

#include "cfg_priv.h"

/*----------------------------------------------------------------------*/

/**/

int cfg_close(

#    if __STDC__
    config_t *cfgs)
#    else
    cfgs)

    config_t *cfgs;
#    endif

{
/*
 * Function: cfg_close.c
 * Version : 1.0
 * Created : 27-Oct-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function cfg_close() is deallocates the data structures.
 *
 *    Invocation:
 *
 *        status = cfg_close(cfgs);
 *
 *    where
 *
 *        <cfgs>              - I
 *            The config data structure to deallocate.
 *
 *        <status>            - O
 *            This function always returns CFG_OK.
 *
 * Modification History
 *
 * Variables Used
 */

    char *data = NULL;

/*
 * Main part of function.
 */

    while ((data = que_pop_tail(&cfgs->list)) != NULL) {

        free(data);

    }

    free(cfgs->filename);

    return(CFG_OK);

}

