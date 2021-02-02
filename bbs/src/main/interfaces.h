
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

#ifndef _INTERFACES_H
#define _INTERFACES_H

#include "error_trace.h"

extern int bbs_run(error_trace_t *);
extern int bbs_logoff(error_trace_t *);

/* access */

extern int known_room(room_search_t *);

#endif

