
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

#ifndef _FINDS_H
#define _FINDS_H

#include "bbs/src/jam/jam.h"
#include "bbs/src/node/node.h"
#include "bbs/src/room/room.h"
#include "bbs/src/user/user.h"

extern int find_rooms_all(void *, int, room_base_t *);
extern int find_rooms_subsys(void *, int, room_base_t *);
extern int find_rooms_messages(void *, int, room_base_t *);
extern int find_rooms_networked(void *, int, room_base_t *);
extern int find_room_by_number(void *, int, room_base_t *);
extern int find_room_by_conference(void *, int, room_base_t *);

extern int find_users_all(void *, void *);
extern int find_user_by_name(void *, void *);
extern int find_user_by_number(void *, void *);

extern int find_nodes_all(void *, void *);
extern int find_node_by_number(void *, void *);
extern int find_nodes_active(void *, void *);

extern int find_index_all(ulong, jam_index_t *);
extern int find_index_by_user(ulong, jam_index_t *);

#endif

