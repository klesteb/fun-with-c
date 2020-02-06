
#include <ssdef.h>
#define __NEW_STARLET 1
#include <smgdef.h>
#undef __NEW_STARLET
#include <descrip.h>
#include <lib$routines.h>
#include <smg$routines.h>
#include <starlet.h>
#include <string.h>

#include "vms.h"
#include "win_defines.h"
#include "win_private.h"

/*-Prototypes-----------------------------------------------------------*/

extern char *center(char *, int);

/*----------------------------------------------------------------------*/

void win_display_title(char *text) {
/*
 * Function: win_display_title.c
 * Version : 1.0
 * Created : 27-Jun-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This routine will print a title in the title area on the screen.
 *
 * Modification History
 *
 * Variables Used
 */

    long stat;
    char *title;

    struct dsc$descriptor title_dsc;

/*
 * Main part of function.
 */

    title = center(text, init.cols / 2);
    SET_SDESC(title_dsc, title, strlen(title));

    stat = smg$put_line_highwide(&init.title_display_id, &title_dsc);
    vms_check_status(stat);

}

