
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

/**
 * @file messages.h
 * @author Kevin L. Esteb (kevin@kesteb.us)
 * @date 15-Nov-2019
 * 
 * @brief A data structure used to hold messages.
 * 
 * @par Description
 * This data structure is used to messages and there associated
 * codes. This is used for displaying messages within applications. 
 * 
 * @struct messages_t messages.h "include/messages.h"
 * 
 **/

typedef struct _messages_s {
    int code;                  /**< the message code */
    char *text;                /**< the message text */
} messages_t;

#endif

