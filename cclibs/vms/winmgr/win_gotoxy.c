
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

void win_gotoxy(long row, long col, win_r_window *win) {
/*
 * Function: win_gotoxy.c
 * Version : 1.0
 * Created : 27-Jun-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This subprogram will position the cursor at "row", "col" for the 
 *    designated window.
 *
 * Modification History
 *
 * Variables Used
 */

    long stat;

/*
 * Main part of function.
 */

    stat = smg$set_cursor_abs(&win->display_id, &row, &col);
    vms_check_status(stat);

}
