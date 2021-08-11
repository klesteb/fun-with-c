
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

#include "bbs/src/main/bbs_includes.h"

/* global items ---------------------------------------------------------- */

extern rms_t *nodes;
extern rms_t *users;
extern room_t *rooms;
extern tracer_t *dump;
extern errors_t *errs;
extern rms_t *profiles;
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
extern int doornum;
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
extern char *workpath;
extern char *networknode;
extern char *humannode;
extern char *serialnum;

extern char *axdefs[8];
extern error_code_t bbs_errors[7];

/*-------------------------------------------------------------------------*/

#endif

