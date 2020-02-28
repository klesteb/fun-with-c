
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

typedef struct _status_s {
    PjlHandle handle;
    int (*read)(PjlHandle);
} status_t;

NxInputId status_id;           /* id for status processor   */
NxWorkProcId file_id;          /* id for file processor     */

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

static int _status_handler(NxAppContext context, NxInputId id, int source, void *data) {

    int stat = ERR;
    status_t *status = (status_t *)data;

    if ((stat = status->read(status->handle)) != OK) {

        NxRemoveInput(NULL, status_id);

    }
    
    return stat;

}

/*----------------------------------------------------------------------*/

int pjl_print(

#if __STDC__
    PjlHandle handle, char *filename, 
    int (*file_read)(FILE *, void *, int),
    int (*status_read)(PjlHandle))
#else
    handle, filename, file_read, status_read)

    PjlHandle handle;
    char *filename;
    int (*file_read)(FILE *, void *, int);
    int (*status_read)(PjlHandle);
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
 *        <status_reader>     - I
 *            A callback to read status messages from the printer.
 * 
 *        <status>            - O
 *            This function will always return 0.
 *
 * Modification History
 *
 * Variables Used
 */

    int fd;
    int stat = OK;
    FILE *fp = NULL;
    file_t *file = NULL;
    status_t *status = NULL;

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

    if (status_read != NULL) {

        if ((status = (status_t *)xmalloc(sizeof(status_t))) == NULL) {

            stat = ERR;
            vperror("(pjl_print) Unable to alloc status reader memory\n.");
            goto fini;

        }

        fd = lfn_fd(handle->stream);

        status->handle = handle;
        status->read = status_read;

        status_id = NxAddInput(NULL, fd, NxInputReadMask, _status_handler, (void *)status);

    }

    file_id = NxAddWorkProc(NULL, _file_handler, (void *)file);

    NxMainLoop(NULL);

    fini:
    fclose(fp);
    if (file != NULL) free(file);
    if (status != NULL) free(status);

    return stat;

}

