
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
/* defination                                                     */
/*----------------------------------------------------------------*/

typedef struct _events_s {
    int type;
    void *data;
} events_t;

typedef struct _kevent_s {
    int keycode;
} KEVENT;

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

#define EVENT_K_KEYBOARD 1
#define EVENT_K_MOUSE    2
#define EVENT_K_EXIT     3
#define EVENT_K_MESSAGE  4
#define EVENT_K_HELP     5
#define EVENT_K_RUN      6

#endif

