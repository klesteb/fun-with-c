
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

int cfg_util_debug = 0;          /* Global debug switch (1/0 = yes/no). */

/*----------------------------------------------------------------------*/

char *trim(char *);
void *xmalloc(int);

/*----------------------------------------------------------------------*/

/**/

int section_find(

#    if __STDC__
    void *data, void *section)
#    else
    data, section)

    void *data;
    void *section;
#    endif

{
/*
 * Function: section_find.c
 * Version : 1.0
 * Created : 01-Nov-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function section_find() will compare the wanted section name to 
 *    current data item. This routine is private to the CFG utilities.
 *
 *    Invocation:
 *
 *        status = section_find(data, section);
 *
 *    where
 *
 *        <data>              - I
 *            The data to be compared.
 *
 *        <section>           - I
 *            The section to be compared too.
 *
 *        <status>            - O
 *            This function will return FALSE if there is a match.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    if (data != NULL) {

        if (strnicmp(section, data, strlen(section)) == 0) return(FALSE);

    }

    return(TRUE);

}

/**/

int cfg_open(

#    if __STDC__
    char *filename, config_t *cfgs)
#    else
    filename, cfgs)

    char *filename;
    config_t *cfgs;
#    endif

{
/*
 * Function: cfg_open.c
 * Version : 1.0
 * Created : 27-Oct-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function cfg_open() will create or open an existing configuration 
 *    file. On succesful completion CFG_OK will be returned, otherwise errno.
 *
 *    Invocation:
 *
 *        status = cfg_open(filename, &cfgs);
 *
 *    where
 *
 *        <filename>              - I
 *            The filename of the configuration data.
 *
 *        <cfgs>                  - O
 *            The allocated configuration data structure.
 *
 *        <status>                - O
 *            This function will return CFG_OK on successor ERRNO.
 *
 * Modification History
 *
 * Variables Used
 */

    FILE *fp = NULL;
    char *data = NULL;

/*
 * Main part of function.
 */

    cfgs->flags = CFG_K_OPENED;
    cfgs->filename = strdup(filename);

    if ((fp = fopen(filename, "r")) == NULL) {

        errno = 0;

        if ((fp = fopen(filename, "w")) == NULL) {

            return(errno);

        }

        cfgs->flags = CFG_K_NEWFILE;

    }

    que_init(&cfgs->list);
    data = xmalloc(CFGBUF + 1);

    while ((fgets(data, CFGBUF, fp)) != NULL) {

        data = trim(data);
        que_push_tail(&cfgs->list, data);
        data = xmalloc(CFGBUF + 1);

    }

    fclose(fp);

    return(CFG_OK);

}

