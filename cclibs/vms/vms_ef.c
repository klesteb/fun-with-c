
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 2000 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include <starlet.h>
#include <lib$routines.h>

#include "vms.h"
#include "bitops.h"

/*----------------------------------------------------------------------*/

ULONG vms_set_ef_mask(ULONG ef, ULONG *mask) {
/*
 * Function: vms_set_ef_mask.c
 * Version : 1.0
 * Created : 19-Jan-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This funcion will set the bit mask for the sys$wflor() or 
 *    sys$wfland() routines.
 *
 * Modification History
 *
 * Variables Used
 */

    int bit;

/*
 * Main part of function.
 */

    if (ef >= 96) {

        bit = ef - 96;
        *mask = bit_set(*mask, bit);

    } else if (ef >= 64) {

        bit = ef - 64;
        *mask = bit_set(*mask, bit);

    } else if (ef >= 32) {

        bit = ef - 32;
        *mask = bit_set(*mask, bit);

    } else {

        *mask = bit_set(*mask, ef);

    }

    return(*mask);

}

/**/

ULONG vms_get_ef(void) {
/*
 * Function: vms_get_ef.c
 * Version : 1.0
 * Created : 19-Jan-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will get an event flag and then clear it.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;
    ULONG flag;

/*
 * Main part of function.
 */

    stat = lib$get_ef(&flag);
    vms_check_status(stat);

    stat = sys$clref(flag);
    vms_check_status(stat);

    return(flag);

}

/**/

void vms_set_ef(ULONG ef) {
/*
 * Function: vms_set_ef.c
 * Version : 1.0
 * Created : 19-Jan-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will set the event flag.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;

/*
 * Main part of function.
 */

    stat = sys$setef(ef);
    vms_check_status(stat);

}

/**/

void vms_free_ef(ULONG ef) {
/*
 * Function: vms_free_ef.c
 * Version : 1.0
 * Created : 19-Jan-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will free the event flag.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;

/*
 * Main part of function.
 */

    stat = lib$free_ef(&ef);
    vms_check_status(stat);

}

/**/

void vms_clear_ef(ULONG ef) {
/*
 * Function: vms_clear_ef.c
 * Version : 1.0
 * Created : 19-Jan-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will clear the event flag.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;

/*
 * Main part of function.
 */

    stat = sys$clref(ef);
    vms_check_status(stat);

}

/**/

int vms_check_ef(ULONG ef) {
/*
 * Function: vms_check_ef.c
 * Version : 1.0
 * Created : 19-Jan-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will read the event flag and return the status.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;
    ULONG state;

/*
 * Main part of function.
 */

    stat = sys$readef(ef, &state);
    vms_check_status(stat);

    return(stat);

}

/**/

int vms_wait_ef(ULONG ef, ULONG mask, int style) {
/*
 * Function: vms_wait_ef.c
 * Version : 1.0
 * Created : 26-Sep-2001
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will wait for the indiated event flag and 
 *    return the status.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;

/*
 * Main part of function.
 */

    if (style == ef_a_or) {

        stat = sys$wflor(ef, mask);
        vms_check_status(stat);

    } else if (style == ef_a_and) {

        stat = sys$wfland(ef, mask);
        vms_check_status(stat);

    }

    return(stat);

}

