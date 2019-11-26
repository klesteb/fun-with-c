
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

#ifndef _ITEM_LIST_H
#define _ITEM_LIST_H

#include "data_types.h"

/**
 * @file item_list.h
 * @author Kevin L. Esteb (kevin@kesteb.us)
 * @date 15-Nov-2019
 * 
 * @brief A data structure used to pass multiple parameters.
 * 
 * @par Description
 * This data structure was derived from OpenVMS, where it is used to
 * pass parameters to System Services. This structure can also be used
 * to return data to the caller.
 * 
 * @struct item_list_t item_list.h "include/item_list.h"
 * 
 **/
typedef struct _item_list_s {
    UWORD buffer_length;       /**< the length of the buffer                 */
    UWORD item_code;           /**< the nemonic for what the data is         */
    void *buffer_address;      /**< the address of the buffer                */
    void *return_address;      /**< the address of the returned datas length */
} item_list_t;

/**
 * @def SET_ITEM(x,a,b,c,d) 
 * A macro to help fill in the fields.
 **/
#define SET_ITEM(x,a,b,c,d) {(x).item_code=(a);\
                             (x).buffer_address=(b);\
                             (x).buffer_length=(c);\
                             (x).return_address=(d);}
#endif

