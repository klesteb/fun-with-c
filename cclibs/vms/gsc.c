
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

#include <stdio.h>
#include <string.h>

#include <rms.h>
#include <descrip.h>
#include <psldef.h>
#include <secdef.h>
#include <ssdef.h>
#include <lckdef.h>
#include <stsdef.h>
#include <starlet.h>
#include <lib$routines.h>

#include "vms.h"
#include "gsc.h"

#define P0SPACE (0x0200)

/*----------------------------------------------------------------------*/

ULONG gsc_exhandler(ULONG *exit_status, void *data);

/*----------------------------------------------------------------------*/

/**/

int gsc_open(char *filename, char *sec_name, int sec_size, GblSec *sec) {
/*
 * Function: gsc_open.c
 * Version : 1.0
 * Created : 19-Nov-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function gsc_open() will create a global section with backing store. 
 *
 *    Invocation
 *
 *        status = gsc_open(filename, sec_name, sec_size, &sec);
 *
 *    Where
 *
 *        <filename>          - I
 *            The filename for the backing store.
 *
 *        <sec_name>          - I
 *            The name of the global section.
 *
 *        <sec_size>          - I
 *            The size of the global section.
 *
 *        <sec>               - O
 *            The data structure for the global section.
 *
 *        <status>            - O
 *            The VMS status of creating the lock with the lock manager.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;
    int file_size;
    ULONG lkmode;
    ULONG flags = 0;
    ULONG acmode;
    char *defname = "SYS$SCRATCH:.GSC";
    ULONG InAdr1[2] = {P0SPACE,P0SPACE};
    struct dsc$descriptor sec_name_dsc;
    struct FAB fab;
    LKSB lksb;

/*
 * Main part of function.
 */

    /* Calculate a size for the backing file.                           */

    file_size = ((sec_size / 512) + 1);

    /* Truncate the global section name to 31 characters. This is the   */
    /* size limit on the resource name for the lock manager.            */

    strncpy(sec->sec_name, sec_name, 31);
    sec->sec_name[31] = '\0';

    SET_SDESC(sec_name_dsc, sec->sec_name, strlen(sec_name));

    fab = cc$rms_fab;

    fab.fab$l_alq = file_size;
    fab.fab$b_fac = 0;
    fab.fab$l_fop = (FAB$M_UFO | FAB$M_CIF);
    fab.fab$b_shr = (FAB$M_UPI | FAB$M_SHRPUT | FAB$M_SHRGET | FAB$M_SHRUPD);

    fab.fab$l_fna = filename;
    fab.fab$b_fns = strlen(filename);
    fab.fab$l_dna = defname;
    fab.fab$b_dns = strlen(defname);

    /* Create or open the specified file -- it is created if            */
    /* not found, else it is opened.  (See the FAB$M_CIF bit.)          */

    stat = sys$create(&fab, 0,0);
    vms_check_status(stat);

    sec->chan = fab.fab$l_stv;

    /* Map the global section over this file. The size of the global    */
    /* section is determined by the size of the backing file.           */

    flags = (SEC$M_EXPREG | SEC$M_WRT | SEC$M_GBL);
    acmode = PSL$C_USER;

    stat = sys$crmpsc(&InAdr1, &sec->sec_address, acmode, 
                      flags, &sec_name_dsc, 0,0, sec->chan, 0,0,0,0);
    vms_check_status(stat);

    /* Create a lock with the lock manager. This is so we can           */
    /* coordinate access to the global section.                         */

    lkmode = LCK$K_NLMODE;

    stat = sys$enqw(0, lkmode, &lksb, 0, &sec_name_dsc, 0,0,0, acmode, 0,0);
    vms_check_status(stat);

    sec->lock_id = lksb.lock_id;

    /* Declare an exception handler to flush the global section.        */

    SET_ECB(sec->ecb, gsc_exhandler, &sec->status, sec);

    stat = sys$dclexh(&sec->ecb);
    vms_check_status(stat);

    return(lksb.stat);

}

/**/

int gsc_flush(GblSec *sec) {
/*
 * Function: gsc_flush.c
 * Version : 1.0
 * Created : 19-Nov-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function gsc_flush() will flush the gloabl section to the backing 
 *    store.
 *
 *    Invocation
 *
 *        status = gsc_flush(section);
 *
 *    Where
 *
 *        <section>           - I
 *            The structure created by gsc_open().
 *
 *        <status>            - O
 *            The VMS status of writing the global section to the backing 
 *            store.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;
    IOSB iosb;

/*
 * Main part of function.
 */

    stat = sys$updsecw(sec->sec_address, 0, PSL$C_USER, 0,0, &iosb, 0,0);
    vms_check_status(stat);
    vms_check_status(iosb.gsc.stat);

    return(stat);

}

/**/

int gsc_close(GblSec *sec) {
/*
 * Function: gsc_close.c
 * Version : 1.0
 * Created : 19-Nov-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function gsc_close() will deallocate the virtual memory and deassign 
 *    the associated channel. This will also flush the global section
 *    to the backing store.
 *
 *    Invocation
 *
 *        status = gsc_close(&section);
 *
 *    Where
 *
 *        <section>           - I
 *            The data structure returned from gsc_open().
 *
 *        <status>            - O
 *            This function will always return 0.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;
    ULONG acmode = PSL$C_USER;

/*
 * Main part of function.
 */

    if (sec->active == TRUE) {

        stat = sys$deltva(sec->sec_address, 0, acmode);
        vms_check_status(stat);

        stat = sys$dassgn(sec->chan);
        vms_check_status(stat);

        stat = sys$canexh(&sec->ecb);
        vms_check_status(stat);

        sec->active = FALSE;
    
    }

    return(0);

}

/**/

ULONG gsc_exhandler(ULONG *exit_status, void *data) {
/*
 * Function: gsc_exhandler.c
 * Version : 1.0
 * Created : 19-Nov-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function gsc_exhandler() is a cloak for the gsc_close() function. It
 *    is called when the program abnormally exits.
 *
 *    Invocation
 *
 *        status = gsc_exhandler(stat, data);
 *
 *    Where
 *
 *        <status>            - I
 *            The VMS status code for the main program.
 *
 *        <data>              - I
 *            The data structure returned from gsc_open().
 *
 *        <status>            - O
 *            This function will always return 0.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;
    ULONG acmode = PSL$C_USER;
    GblSec *sec = NULL;

/*
 * Main part of function.
 */

    sec = data;

    stat = sys$deltva(sec->sec_address, 0, acmode);
    vms_check_status(stat);

    stat = sys$dassgn(sec->chan);
    vms_check_status(stat);

    sec->active = FALSE;

    return(stat);

}

/**/

void *gsc_associate(GblSec *sec) {
/*
 * Function: gsc_associate.c
 * Version : 1.0
 * Created : 19-Nov-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function gsc_associate() will associate the starting virtual address 
 *    of the global section to an address of a data structure within the 
 *    main program.
 *
 *    Invocation
 *
 *        data = gsc_associate(&section);
 *
 *    Where
 *
 *        <section>           - I
 *            The data structure returned by gsc_open().
 *
 *        <data>              - O
 *            The starting address of the virtual memory location.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    return((void *)sec->sec_address[0]);

}

/**/

int gsc_lock(GblSec *sec) {
/*
 * Function: gsc_lock.c
 * Version : 1.0
 * Created : 19-Nov-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function gsc_lock() will lock the global section. This should be
 *    done whenever the global section is accessed. This will coordinate 
 *    access so that everybody has a consistent view of the shared data.
 *
 *    Invockation
 *
 *        status = gsc_lock(&section);
 *
 *    Where
 *
 *        <section>           - I
 *            Is the data structure returned by gsc_open().
 *
 *        <status>            - O
 *            Is the VMS status of the system request.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;
    ULONG lkmode;
    ULONG flags;
    LKSB lksb;

/*
 * Main part of function.
 */

    lkmode = LCK$K_EXMODE;
    lksb.lock_id = sec->lock_id;
    flags = LCK$M_CONVERT;

    stat = sys$enqw(0, lkmode, &lksb, flags, 0,0,0,0,0,0,0);
    vms_check_status(stat);

    return(lksb.stat);

}

/**/

int gsc_unlock(GblSec *sec) {
/*
 * Function: gsc_unlock.c
 * Version : 1.0
 * Created : 19-Nov-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function gsc_unlock() will remove the lock from the global section.
 *
 *    Invocation
 *
 *        status = gsc_unlock(&section);
 *
 *    Where
 *
 *        <section>           - I
 *            The data structure returned from gsc_open().
 *
 *        <status>            - O
 *            The VMS status of the system request.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;
    ULONG lkmode;
    ULONG flags;
    LKSB lksb;

/*
 * Main part of function.
 */

    lkmode = LCK$K_NLMODE;
    lksb.lock_id = sec->lock_id;
    flags = LCK$M_CONVERT;

    stat = sys$enqw(0, lkmode, &lksb, flags, 0,0,0,0,0,0,0);
    vms_check_status(stat);

    return(lksb.stat);

}

