
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

int cfg_save(

#    if __STDC__
    config_t *cfgs)
#    else
    cfgs)

    config_t *cfgs;
#    endif

{
/*
 * Function: cfg_save.c
 * Version : 1.0
 * Created : 27-Oct-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function cfg_save() will save the current configuration into the
 *    configuration file.
 *
 *    Invocation:
 *
 *        status = cfg_save(cfgs);
 *
 *    where
 *
 *        <cfgs>              - I
 *            Is the data structure returned by cfg_open().
 *
 *        <status>            - O
 *            This function will return CFG_OK or ERRNO if there were any 
 *            problems.
 * 
 * Modification History
 *
 * Variables Used
 */

    FILE *fp = NULL;
    char *data = NULL;
    int stat = CFG_OK;

/*
 * Main part of function.
 */

    if ((fp = fopen(cfgs->filename, "w")) == NULL) {

        return(errno);

    }

    for (data = que_first(&cfgs->list);
         data != NULL;
         data = que_next(&cfgs->list)) {
    
        errno = 0;
        fprintf(fp,"%s\n", data);

        if (errno != 0) {

            stat = errno;
            break;

        }

    }

    fclose(fp);

    return(stat);

}

