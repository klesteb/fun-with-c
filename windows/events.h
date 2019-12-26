
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

#ifndef _EVENTS_H_
#define _EVENTS_H_

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

typedef struct _event_s event_t;

struct _event_s {
    object_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(event_t *, event_t *);
    int (*_override)(event_t *, item_list_t *);
    int type;
    void *data;
};

/*----------------------------------------------------------------*/
/* defination                                                     */
/*----------------------------------------------------------------*/

typedef struct _event_s {
    int type;
    void *data;
} event_t;

typedef struct _kevent_s {
    int keycode;
} KEVENT;

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

#define EVENT_K_KEYBOARD 1
#define EVENT_K_MOUSE    2

/*----------------------------------------------------------------*/
/* interface                                                      */
/*----------------------------------------------------------------*/

extern event_t *event_create(int);
extern int event_destroy(event_t *);
extern int event_compare(event_t *, event_t *);
extern int event_override(event_t *, item_list_t *);

#endif

