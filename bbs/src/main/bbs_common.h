
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

#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "msgs.h"
#include "node.h"
#include "room.h"
#include "user.h"
#include "when.h"
#include "event.h"
#include "finds.h"
#include "files.h"
#include "bitops.h"
#include "errors.h"
#include "tracer.h"
#include "window.h"
#include "fnm_util.h"
#include "misc/misc.h"
#include "workbench.h"
#include "bbs_error_codes.h"

/* global items */

extern node_t *nodes;
extern user_t *users;
extern room_t *rooms;
extern tracer_t *dump;
extern errors_t *errs;
extern event_t *events;
extern workbench_t *workbench;

extern int xnode;
extern int sysop;
extern int qroom_index;
extern int user_index;
extern int qnode_index;
extern char *username;

extern room_base_t qroom;
extern user_base_t useron;
extern node_base_t qnode;

#endif

