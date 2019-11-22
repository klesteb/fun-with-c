
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2019 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#ifndef _MESSAGE_H
#define _MESSAGE_H

#include "object.h"
#include "que_util.h"
#include "item_list.h"
#include "messages.h"

/*----------------------------------------------------------------*/
/* klass defination                                               */
/*----------------------------------------------------------------*/

typedef struct _message_s message_t;

struct _message_s {
    object_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(message_t *, message_t *);
    int (*_override)(message_t *, item_list_t *);
    int (*_del_message)(message_t *, int);
    int (*_set_message)(message_t *, int, char *);
    int (*_add_message)(message_t *, int, char *);
    int (*_get_message)(message_t *, int, char *, int);
    int (*_load_messages)(message_t *, messages_t *, int);
    queue messages;
};

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

#define MESSAGE(x) ((message_t *)(x))

#define MESSAGE_M_DESTRUCTOR 1

/*-------------------------------------------------------------*/
/* interface                                                   */
/*-------------------------------------------------------------*/

extern message_t *message_create(item_list_t *);
extern int message_destroy(message_t *);
extern int message_compare(message_t *, message_t *);
extern int message_override(message_t *, item_list_t *);
extern int message_del(message_t *, int);
extern int message_add(message_t *, int, char *);
extern int message_set(message_t *, int, char *);
extern int message_get(message_t *, int, char *, int);
extern int message_load(message_t *, messages_t *, int);

#endif

