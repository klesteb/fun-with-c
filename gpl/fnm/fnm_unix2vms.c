
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 2002 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include "fnm_priv.h"

/*----------------------------------------------------------------------*/

/**/

void  fnm_unix2vms (
#    if  __STDC__
        char *unix_path,
        char *vms_path,
        int outlen)
#    else
        unix_path, vms_path, outlen)

        char *unix_path;
        char *vms_path;
        int outlen;
#    endif
{
/*
 * Function: fnm_unix2vms.c
 * Version : 1.0
 * Created : 29-May-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function fnm_unix2vms() will convert a UNIX directory path to VMS
 *    format. This functions assumes that the first part of the path is
 *    a VMS disk device.
 *
 *        example
 *            UNIX => trex:/d2/ssg_dev/source/movefile/nftmon.log.1
 *            VMS  => trex::d2:[ssg_dev.source.movefile]nftmon.log.1
 *
 *    Invocation
 *
 *        fnm_unix2vms(unix_path, vms_path, sizeof(vms_path));
 *
 *    where
 *
 *        <unix_path>         - I
 *            The UNIX path to be converted to VMS syntax.
 *
 *        <vms_path>          - O
 *            The path in VMS syntax.
 *
 *        <size>              - I
 *            The size of vms_path.
 *
 * Modification History
 *
 * Variables Used
 */

    int x = 0;
    int y = 0;
    int len;
    int first = TRUE;
    char *node = NULL;
    char *directory = NULL;
    char *name = NULL;
    char *extension = NULL;
    char *version = NULL;
    FileName fname;

/*
 * Main part of function.
 */

    fname = fnm_create(FALSE, unix_path, NULL);

    node = fnm_node(fname);
    directory = fnm_directory(fname);
    name = fnm_name(fname);
    extension = fnm_extension(fname);
    version = fnm_version(fname);

    if (strlen(node) > 0) {                /* The node will have one ":"   */

        strcpy(vms_path, node);
        strcat(vms_path, ":");
        x = strlen(vms_path);

    }

    first = TRUE;

    if ((len = strlen(directory)) > 0) {

        for (; y < (len - 1); y++, x++) {

            if (directory[y] == '/') {

                if (first) {

                    first = FALSE;
                    y++;

                    for (; y < len; y++, x++) {

                        if (directory[y] == '/') {

                            vms_path[x] = ':';
                            x++;
                            vms_path[x] = '[';
                            break;

                        } else vms_path[x] = directory[y];

                    }

                } else vms_path[x] = '.';

            } else vms_path[x] = directory[y];

        }

        vms_path[x] = ']';
        x++;

    }
        
    if ((len = strlen(name)) > 0) {

        for (y = 0; y < len; y++, x++) {

            vms_path[x] = name[y];

        }

    }

    if ((len = strlen(extension)) > 0) {

        for (y = 0; y < len; y++, x++) {

            vms_path[x] = extension[y];

        }

    }

    if ((len = strlen(version)) > 0) {

        for (y = 0; y < len; y++, x++) {

            vms_path[x] = version[y];

        }

    }

    vms_path[x] = '\0';

    fnm_destroy(fname);

}

