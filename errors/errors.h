
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

#ifndef _ERRORS_H
#define _ERRORS_H

#include "object.h"
#include "que_util.h"
#include "error_code.h"
#include "error_codes.h"

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

typedef struct _errors_s errors_t;

struct _errors_s {
    object_t parent_klass;
    int   (*ctor)(object_t *, item_list_t *);
    int   (*dtor)(object_t *);
    char *(*_get_text)(errors_t *, int);
    char *(*_get_message)(errors_t *, int);
    int   (*_remove_error_code)(errors_t *, int);
    int   (*_add_error_code)(errors_t *, int, char *, char *);
    int   (*_add_error_codes)(errors_t *, error_code_t *, int);
    queue error_codes;
};

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

#define ERRORS(x) ((errors_t *)(x))

#define ERRORS_K_ERROR_CODES  1

/*-------------------------------------------------------------*/
/* interface                                                   */
/*-------------------------------------------------------------*/

extern errors_t *errors_create(void);
extern int errors_destroy(errors_t *);
extern char *errors_get_text(errors_t *, int);
extern char *errors_get_message(errors_t *, int);
extern int errors_remove_error_code(errors_t *, int);
extern int errors_add_error_code(errors_t *, int, char *, char *);
extern int errors_add_error_codes(errors_t *, error_code_t *, int);

#endif

