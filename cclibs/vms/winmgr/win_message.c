
#include <ssdef.h>
#define __NEW_STARLET 1
#include <smgdef.h>
#undef __NEW_STARLET
#inclide <descrip.h>
#include <lib$routines.h>
#include <smg$routines.h>
#include <starlet.h>
#include <string.h>

#include "vms.h"
#include "win_defines.h"
#include "win_private.h"

/*-Prototypes-----------------------------------------------------------*/

/*----------------------------------------------------------------------*/

void win_message(char *text, int bell) {
/*
 * Function: win_message.c
 * Version : 1.0
 * Created : 28-Jun-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This routine will print a message in the message area on the screen. 
 *    It will then wait two seconds and clear the message area and exit. 
 *    It will also ring the terminals bell if so desired.
 *
 * Modification History
 *
 * Variables Used
 */

    long stat;

    struct dsc$descriptor text_dsc;

/*
 * Main part of function.
 */

    SET_SDESC(text_dsc, text, strlen(text));

    stat = smg$put_line(&init.msg_display_id, &text_dsc);
    vms_check_status(stat);

    if (bell) {

        stat = smg$ring_bell(&init.msg_display_id);
        vms_check_status(stat);

    }

    sleep(2);

    stat = smg$erase_display(&init.msg_display_id);
    vms_check_status(stat);

}

