
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

void win_clear_message(void) {
/*
 * Function: win_clear_message.c
 * Version : 1.0
 * Created : 27-Jun-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This routine will clear the message area on the screen.
 *
 * Modification History
 *
 * Variables Used
 */

    long stat;

/*
 * Main part of function.
 */

    stat = smg$erase_display(&init.msg_display_id);
    vms_check_status(stat);

}

