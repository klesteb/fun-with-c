
/*---------------------------------------------------------------------------*/
/*              Copyright (c) 2020 by Kevin L. Esteb                         */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include "nix_util.h"
#include "pjl_priv.h"

/*----------------------------------------------------------------------*/
/* private data                                                         */
/*----------------------------------------------------------------------*/

typedef struct _file_s {
    FILE *fp;
    PjlHandle handle;
    int (*read)(FILE *, void *, int);
} file_t;

NxInputId read_id;             /* id for file read processor      */
NxWorkProcId file_id;          /* id for network status processor */

/*----------------------------------------------------------------------*/
/* private routines                                                     */
/*----------------------------------------------------------------------*/

static int _file_handler(NxAppContext context, NxWorkProcId id, void *data) {

    int stat = ERR;
    char content[PJL_K_BUFSIZ];
    int size = PJL_K_BUFSIZ - 1;
    file_t *file = (file_t *)data;

    memset(content, '\0', PJL_K_BUFSIZ);

    if ((stat = file->read(file->fp, (void *)content, size)) != OK) {

        vperror("(pjl_print) Unable to read content from file.\n");
        goto fini;

    }

    if ((stat = _pjl_write(file->handle, content, size)) != OK) {

        vperror("(pjl_print) Unable to send data to printer.\n");
        goto fini;

    }

    file_id = NxAddWorkProc(NULL, &_file_handler, (void *)data);

    fini:
    return stat;

}

/* static int _status_handler(NxAppContext context, NxInputId id, int source, void *data) { */
    
/*     return 0; */
    
/* } */

/*----------------------------------------------------------------------*/

int pjl_print(

#if __STDC__
    PjlHandle handle, char *filename, int (*file_read)(FILE *, void *, int))
#else
    handle, filename, file_read)

    PjlHandle handle;
    char *filename;
    int (*file_read)(FILE *, void *, int);
#endif

{
/*
 * Function: pjl_print.c
 * Version : 1.0
 * Created : 26-Feb-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function pjl_print() will send a file to a printer.
 *
 *    Invocation:
 *
 *        status = pjl_printf(handle, filename, file_reader);
 *
 *    where
 *
 *        <handle>            - I
 *            The handle for subsequent operations.
 *
 *        <filename>          - I
 *            The file to send to the printer.
 * 
 *        <file_reader>       - I
 *            A callback to read and process the file.
 * 
 *        <status>            - O
 *            This function will always return 0.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = OK;
    FILE *fp = NULL;
    file_t *file = NULL;

/*
 * Main part of function.
 */

    if ((handle == NULL) || (filename == NULL) || (file_read == NULL)) {

        stat = ERR;
        vperror("(pjl_print) Invalid parameters.\n");
        goto fini;

    }

    if ((fp = fopen(filename, "r")) == NULL) {

        stat = ERR;
        vperror("(pjl_print) Unable to open file: %s\n", filename);
        goto fini;

    }

    if ((file = (file_t *)xmalloc(sizeof(file_t))) == NULL) {

        stat = ERR;
        vperror("(pjl_print) Unable to alloc file reader memory\n.");
        goto fini;

    }

    file->fp = fp;
    file->handle = handle;
    file->read = file_read;

    file_id = NxAddWorkProc(NULL, &_file_handler, (void *)file);

    NxMainLoop(NULL);

    free(file);

    fini:
    return stat;

}

