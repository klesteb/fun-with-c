
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

int cfg_set(

#    if __STDC__
    config_t *cfgs, char *section, char *key, char *value)
#    else
    cfgs, section, key, value)

    config_t *cfgs;
    char *section;
    char *key;
    char *value;
#    endif

{
/*
 * Function: cfg_set.c
 * Version : 1.0
 * Created : 27-Oct-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function cfg_set() will set the particular key/value pair in a given 
 *    section.
 *
 *    The following rules apply:
 *
 *        1) If the configuration has a CFG_K_NEWFILE flag then create a
 *           section with the key/value entry. Update the flag to 
 *           CFG_K_UPDATED and return CFG_OK.
 *
 *        2) If the section is a NULL value return CFG_ERROR.
 *
 *        3) If the key is a NULL value return CFG_ERROR.
 *
 *    Invocation:
 *
 *        status = cfg_set(cfgs, section, key, value);
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
 *            The key wanted.
 *
 *        <value>             - I
 *            A value for the key.
 *
 *        <status>            - O
 *            This function will return either CFG_OK or CFG_ERROR.
 *
 * Modification History
 *
 * Variables Used
 */

    char *secbuf = NULL;
    char *store = NULL;
    char *data = NULL;
    int stat = CFG_ERROR;

/*
 * Main part of function.
 */

    if (cfgs->flags == CFG_K_NEWFILE) return(-1);
    if (strlen(section) == 0) return(-1);
    if (strlen(key) == 0) return(-1);

    secbuf = xmalloc(CFGBUF + 1);
    store = xmalloc(CFGBUF + 1);
    sprintf(secbuf, "[%s]", section);
    sprintf(store, "%s=%s", key, value);

    if (cfgs->flags == CFG_K_NEWFILE) {

        que_push_tail(&cfgs->list, " ");
        que_push_tail(&cfgs->list, secbuf);
        que_push_tail(&cfgs->list, store);
        cfgs->flags = CFG_K_UPDATED;
        stat = CFG_OK;

    } else {

        if (que_find(&cfgs->list, secbuf, section_find)) {

            for (data = que_next(&cfgs->list);
                 data != '\0';
                 data = que_next(&cfgs->list)) {

                if (data[0] != '[') {

                    if (strnicmp(key, data, strlen(key)) == 0) {

                        data = que_get(&cfgs->list);
                        que_put(&cfgs->list, store);
                        free(data);
                        stat = CFG_OK;
                        break;

                    }

                }

            }

            if (stat != CFG_OK) {

                que_find(&cfgs->list, secbuf, section_find);
                que_insert(&cfgs->list, store);

            }

        }

    }

    free(secbuf);

    return(stat);

}

