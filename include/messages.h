
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

#ifndef _MESSAGES_H_
#define _MESSAGES_H_

typedef struct _messages_s {
    int nemonic;
    char *text;
} messages_t;

/*-------------------------------------------------------------*/
/* default message nemonics                                    */
/*-------------------------------------------------------------*/

#define M_STARTUP  1
#define M_SHUTDOWN 2
#define M_STARTRUN 3
#define M_STOPRUN  4

#endif

