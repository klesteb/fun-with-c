
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2021 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that this copyright notice appears in all copies. The author    */
/*  makes no representations about the suitability of this software for      */
/*  any purpose. It is provided "as is" without express or implied warranty. */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef _ROOM_COMMON_H
#define _ROOM_COMMON_H

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

#define LOBBY    1          /* lobby is always room 1          */
#define MAILROOM 2          /* mail is always room 2           */
#define AIDEROOM 3          /* aide is always room 3           */

#ifndef SYSOP               /* see user.h - these must match   */
#define SYSOP  1            /* this is a bad practice, but i   */
#define QWKNET 2            /* don't want to bleed user stuff  */
#define GUEST  3            /* into this module.               */
#endif

#endif

