
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

void win_clear_title(void) {
/*
 * Function: win_clear_title.c
 * Version : 1.0
 * Created : 27-Jun-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This routine will clear the title in the title area on the screen.
 *
 * Modification History
 *
 * Variables Used
 */

    long stat;

/*
 * Main part of function.
 */

    stat = smg$erase_display(&init.title_display_id);
    vms_check_status(stat);

}

