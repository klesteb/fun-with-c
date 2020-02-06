
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 2000 by Kevin L. Esteb                                     */
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

int cfg_exists(

#    if __STDC__
    config_t *cfgs, char *section)
#    else
    cfgs, section)

    config_t *cfgs;
    char *section;
#    endif

{
/*
 * Function: cfg_exists.c
 * Version : 1.0
 * Created : 24-Oct-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function cfg_exists() will return check to see if a particular secion
 *    exists within a configuration. If successful CFG_OK will be return, 
 *    otherwise CFG_ERROR. 
 *
 *    The following rules apply:
 *
 *        1) If the configuration has a CFG_K_NEWFILE flag then return
 *           CFG_ERROR. No section values have been defined yet.
 *
 *        2) If the section is a NULL value return CFG_ERROR.
 *
 *    Invocation:
 *
 *        status = cfg_exists(cfgs, section);
 *
 *    where
 *
 *        <cfgs>              - I
 *            Is the data structure returned by cfg_open().
 *
 *        <section>           - I
 *            The configuration section wanted.
 *
 *        <status>            - O
 *            This function will return either CFG_OK or CFG_ERROR.
 *
 * Modification History
 *
 * Variables Used
 */

    char cmpbuf[CFGBUF + 1];
    int stat = -1;

/*
 * Main part of function.
 */

    if (cfgs->flags == CFG_K_NEWFILE) return(CFG_ERROR);
    if (strlen(section) == 0) return(CFG_ERROR);

    sprintf(cmpbuf, "[%s]", section);

    stat = que_find(&cfgs->list, cmpbuf, section_find);

    return(stat);

}

