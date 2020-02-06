
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

int cfg_del(

#    if __STDC__
    config_t *cfgs, char *section, char *key)
#    else
    cfgs, section, key)

    config_t *cfgs;
    char *section;
    char *key;
#    endif

{
/*
 * Function: cfg_del.c
 * Version : 1.0
 * Created : 27-Oct-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function cfg_del() will delete an item from the current configuration.
 *
 *    Successful completion will return CFG_OK. An error will return 
 *    CFG_ERROR. The following rules apply.
 *
 *        1) If this is a CFG_K_NEWFILE then return 0. There is no 
 *           configuration information.
 *
 *        2) If the section has a NULL value then return -1. You must have
 *           a section name to delete any keys.
 *
 *        3) If the section is not NULL and the key is NULL then delete
 *           all keys within that section. After that is done then delete
 *           the section heading.
 *
 *        4) If the section is not NULL and the key is not NULL then delete
 *           that key.
 *
 *    Invocation:
 *
 *        status = cfg_del(cfgs, section, key);
 *
 *    where
 *
 *        <cfgs>              - I
 *            Is the data structure returned by cfg_open().
 *
 *        <section>           - I
 *            The configuration section wanted.
 *
 *        <key>               - I
 *            The key to be deleted.
 *
 *        <status>            - O
 *            This function always returns CFG_OK.
 *
 * Modification History
 *
 * Variables Used
 */

    char cmpbuf[CFGBUF + 1];
    char *data = NULL;
    int delall = FALSE;

/*
 * Main part of function.
 */

    if (cfgs->flags == CFG_K_NEWFILE) return(0);
    if (strlen(section) == 0) return(-1);
    if (strlen(key) == 0) delall = TRUE;

    sprintf(cmpbuf, "[%s]", section);

    if (que_find(&cfgs->list, cmpbuf, section_find)) {

        if (delall) {

            do {

                data = que_delete(&cfgs->list);
                free(data);
                data = que_next(&cfgs->list);

            } while (data[0] != '[');

        } else {

            data = que_next(&cfgs->list);

            while (data[0] != '[') {

                if (strnicmp(key, data, strlen(key)) == 0) {

                    free(data);
                    que_delete(&cfgs->list);
                    break;

                }

                data = que_next(&cfgs->list);

            }

        }

    }

    return(CFG_OK);

}

