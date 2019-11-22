
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

#ifndef _ERROR_CODES_H_
#define _ERROR_CODES_H_

typedef struct _error_code_s {
    int ec;
    char *text;
    char *message;
} error_code_t;

/*-------------------------------------------------------------*/
/* error code constants                                        */
/*-------------------------------------------------------------*/

#define E_OK      1000
#define E_INVPARM 1001
#define E_NODATA  1002
#define E_INVOPS  1003
#define E_INVOBJ  1004
#define E_NOLOAD  1005

#endif

