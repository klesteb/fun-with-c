
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

#ifndef _ROOM_MSGS_H
#define _ROOM_MSGS_H

#include "bbs/src/room/handler.h"
#include "bbs/src/room/room_common.h"

/*----------------------------------------------------------------*/
/* constants                                                      */
/*----------------------------------------------------------------*/

/*----------------------------------------------------------------*/
/* interface                                                      */
/*----------------------------------------------------------------*/

extern handler_t *msgs_create(files_t *, char *, int, int, int, tracer_t *);

#endif

