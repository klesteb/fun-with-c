
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

#ifndef _MESSAGE_PRIV_H_
#define _MESSAGE_PRIV_H_

#include "messages.h"

/*-------------------------------------------------------------*/
/* default message table                                       */
/*-------------------------------------------------------------*/

messages_t defaults[] = {
    { M_STARTUP,  "Starting up" },
    { M_SHUTDOWN, "Shutting down" },
    { M_STARTRUN, "Start run" },
    { M_STOPRUN,  "Stop run" }
};

#endif

