
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
#include "room_status.h"
#include "bbs_error_codes.h"

/* global items ---------------------------------------------------------- */

extern rms_t *nodes;
extern rms_t *users;
extern room_t *rooms;
extern rms_t *rstatus;
extern tracer_t *dump;
extern errors_t *errs;
extern event_t *events;
extern workbench_t *workbench;

extern int xnode;
extern int sysop;
extern off_t user_index;
extern off_t qnode_index;
extern char *username;

extern user_base_t useron;
extern node_base_t qnode;

/* config items -----------------------------------------------------------*/

extern int base;
extern int xtimeout;
extern int retries;
extern int roomnum;
extern int nodenum;
extern int usernum;
extern int networked;
extern int creataide;
extern int lobbypost;
extern int makeroom;
extern int initax;
extern int validax;
extern int regiscall;
extern int timelim;
extern char *bbsdir;
extern char *datapath;
extern char *msgpath;
extern char *textpath;
extern char *networknode;
extern char *humannode;
extern char *serialnum;
extern int rstatnum;
extern char *axdefs[8];

#endif

