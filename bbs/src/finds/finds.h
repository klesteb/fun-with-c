
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

#include "node.h"
#include "user.h"
#include "room.h"

extern int find_rooms_all(void *, int, room_base_t *);
extern int find_rooms_messages(void *, int, room_base_t *);
extern int find_rooms_networked(void *, int, room_base_t *);
extern int find_room_by_number(void *, int, room_base_t *);
extern int find_room_by_conference(void *, int, room_base_t *);

extern int find_users_all(void *, user_base_t *);
extern int find_user_by_name(void *, user_base_t *);

extern int find_nodes_all(void *, int, node_base_t *);
extern int find_node_by_number(void *, int, node_base_t *);

#endif

