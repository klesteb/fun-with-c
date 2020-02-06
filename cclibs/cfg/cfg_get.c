
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

int cfg_get(

#    if __STDC__
    config_t *cfgs, char *section, char *key, char *def, char *result)
#    else
    cfgs, section, key, def, result)

    config_t *cfgs;
    char *section;
    char *key;
    char *def;
    char *result;
#    endif

{
/*
 * Function: cfg_get.c
 * Version : 1.0
 * Created : 27-Oct-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function cfg_get() will return the results of a key lookup in a 
 *    particular section. If successful CFG_OK will be return, otherwise
 *    CFG_ERROR. 
 *
 *    The following rules apply:
 *
 *        1) If the configuration has a CFG_K_NEWFILE flag then return
 *           CFG_ERROR. No section/key values have been defined yet.
 *
 *        2) If the section is a NULL value return CFG_ERROR.
 *
 *        3) If the key is a NULL value return CFG_ERROR.
 *
 *        4) If the key/value pair is not found the then default value is
 *           returned as the result.
 *
 *    The result will have all white space trimed from the beginning and 
 *    end of the string. No interpetation will be applied to the result.
 *
 *    Invocation:
 *
 *        status = cfg_get(cfgs, section, key, default, result);
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
 *        <default>           - I
 *            A default value if the key is not found.
 *
 *        <result>            - O
 *            The result of the section/key lookup.
 *
 *        <status>            - O
 *            This function will return either CFG_OK or CFG_ERROR.
 *
 * Modification History
 *
 * Variables Used
 */

    char cmpbuf[CFGBUF + 1];
    char *data = NULL;
    char *holder = NULL;
    int stat = -1;
    int x;
    int y;

/*
 * Main part of function.
 */

    if (cfgs->flags == CFG_K_NEWFILE) return(CFG_ERROR);
    if (strlen(section) == 0) return(CFG_ERROR);
    if (strlen(key) == 0) return(CFG_ERROR);

    sprintf(cmpbuf, "[%s]", section);

    if (que_find(&cfgs->list, cmpbuf, section_find) == QUE_OK) {

        for (data = que_next(&cfgs->list);
             data != '\0';
             data = que_next(&cfgs->list)) {

            if (data[0] != '[') {

                if (strnicmp(key, data, strlen(key)) == 0) {

                    data = que_get(&cfgs->list);
                    x = pos(data, "=", 0);
                    holder = right(data, x + 1);
                    y = pos(holder, "!", 0);
                    if (y > 0) holder = left(holder, y - 1);
                    holder = trim(holder);
                    strcpy(result, holder);
                    free(holder);
                    stat = CFG_OK;
                    break;

                }

            }

        }

        if (stat != CFG_OK) {

            strcpy(result, def);
            stat = CFG_OK;

        }

    }

    return(stat);

}

