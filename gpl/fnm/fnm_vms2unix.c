
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

void  fnm_vms2unix (
#    if  __STDC__
        char *vms_path,
        char *unix_path,
        int outlen)
#    else
        vms_path, unix_path, outlen)

        char *vms_path;
        char *unix_path;
        int outlen;
#    endif
{
/*
 * Function: fnm_vms2unix.c
 * Version : 1.0
 * Created : 04-Apr-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function fnm_vm2unix() will convert a VMS directory path to UNIX 
 *    format.
 *
 *        example
 *            VMS  => mf_root:[000000]nftmon.log;1
 *            VMS  => mf_root:<000000>nftmon.log.1
 *            UNIX => /mf_root/000000/nftmon.log.1
 *
 *    Invocation
 *
 *        fnm_vms2unix(vms_path, unix_path, sizeof(unix_path));
 *
 *    where
 *
 *        <vms_path>          - I
 *            A path in VMS syntax.
 *
 *        <unix_path>         - O
 *            The VMS path converted to Unix syntax.
 *
 *        <size>              - I
 *            The size of unix_path.
 *
 * Modification History
 *
 * Variables Used
 */

    int x = 0;
    int y = 1;
    int len;
    int pathdone = FALSE;

/*
 * Main part of function.
 */

    len = strlen(vms_path);
    if (outlen < len) {

        strncpy(unix_path, vms_path, outlen);
        unix_path[outlen] = '\0';
        return;

    }

    unix_path[0] = '/';

    for (x = 0; x < len; x++) {

        if (vms_path[x] == ':') {

            unix_path[y] = '/';

        } else if ((vms_path[x] == '[') || (vms_path[x] == '<')) {

            continue;

        } else if (vms_path[x] == '.') {

            if (pathdone == TRUE) {

                unix_path[y] = vms_path[x];

            } else unix_path[y] = '/';

        } else if (vms_path[x] == ';') {

            unix_path[y] = '.';

        } else if ((vms_path[x] == ']') || (vms_path[x] == '>')) {

            unix_path[y] = '/';
            pathdone = TRUE;

        } else {

            unix_path[y] = vms_path[x];

        }

        y++;

    }

    unix_path[y] = '\0';

}

