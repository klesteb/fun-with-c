
#include <ssdef.h>
#define __NEW_STARLET 1
#include <smgdef.h>
#undef __NEW_STARLET
#include <descrip.h>
#include <lib$routines.h>
#include <smg$routines.h>
#include <starlet.h>

#include "vms.h"
#include "win_defines.h"
#include "win_private.h"

/*-Prototypes-----------------------------------------------------------*/

/*----------------------------------------------------------------------*/

void win_close_window(win_r_window *win) {
/*
 * Function: win_close_window.c
 * Version : 1.0
 * Created : 23-Jun-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This sub progam will close a display window on the pasteboard
 *    in an consistent manner. 
 *
 * Modification History
 *
 * Variables Used
 */

    long stat;

/*
 * Main part of function.
 */

    /* Erase the display window.                                        */

    stat = smg$erase_display(&win->display_id);
    vms_check_status(stat);

    /* Close the display window.                                        */

    stat = smg$delete_virtual_display(&win->display_id);
    vms_check_status(stat);
    
}

