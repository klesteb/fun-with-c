
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

#ifndef _BBS_PROTOS_H
#define _BBS_PROTOS_H

#include "jam.h"
#include "error_trace.h"

/* bbs_main */

extern char *bbs_version(void);
extern int bbs_init(error_trace_t *);
extern int bbs_run(error_trace_t *);

/* bbs_access */

extern int is_aide(room_base_t *, user_base_t *);
extern int is_sysop(room_base_t *, user_base_t *);
extern int known_room(room_search_t *, user_base_t *);
extern int allowed_in_room(room_search_t *, user_base_t *);

/* bbs_ipc */

extern int bbs_get_status(error_trace_t *);
extern int bbs_send_status(int, error_trace_t *);

/* bbs_rooms */

extern int bbs_load_room(void *, int, error_trace_t *);
extern int bbs_list_rooms(void *, int, error_trace_t *);

/* bbs_ui */

extern int bbs_main_menu(error_trace_t *);
extern int bbs_clear_message(error_trace_t *);
extern int bbs_who(void *, int, error_trace_t *);
extern int bbs_help(void *, int, error_trace_t *);
extern int bbs_system(void *, int, error_trace_t *);
extern int bbs_send_message(const char *, error_trace_t *);

/* bbs_logoff */

extern int bbs_logoff(error_trace_t *);
extern int bbs_logout(void *, int, error_trace_t *);

/* bbs_aide */

extern int bbs_aide_menu(error_trace_t *);

/* bbs_msgs */

extern int bbs_msgs_menu(jam_t *, room_base_t *, error_trace_t *);

/* bbs_utils */

extern int bbs_load_file(char *, queue *, error_trace_t *);

#endif

