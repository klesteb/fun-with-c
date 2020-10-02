
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2020 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#ifndef _FILES_H
#define _FILES_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "object.h"
#include "item_list.h"

/*-------------------------------------------------------------*/
/* klass defination                                            */
/*-------------------------------------------------------------*/

typedef struct _files_s files_t;

struct _files_s {
    object_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(files_t *, files_t *);
    int (*_override)(files_t *, item_list_t *);
    int (*_open)(files_t *, int, mode_t);
    int (*_close)(files_t *);
    int (*_seek)(files_t *, off_t, int);
    int (*_tell)(files_t *, off_t *);
    int (*_read)(files_t *, void *, size_t, int *);
    int (*_write)(files_t *, void *, size_t, int *);
    int (*_gets)(files_t *, char *, int);
    int (*_puts)(files_t *, char *);
};

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

#define FILES(x) ((files_t *)(x))

#define FILES_K_PATH   1
#define FILES_K_LOCKER 2

#define FILES_M_DESTRUCTOR 1
#define FILES_M_OPEN       2
#define FILES_M_CLOSE      3
#define FILES_M_SEEK       4
#define FILES_M_TELL       5
#define FILES_M_READ       6
#define FILES_M_WRITE      7
#define FILES_M_GETS       8
#define FILES_M_PUTS       9

/*-------------------------------------------------------------*/
/* interface                                                   */
/*-------------------------------------------------------------*/

extern files_t *files_create(item_list_t *);
extern int files_destroy(files_t *);
extern int files_compare(files_t *, files_t *);
extern int files_override(files_t *, item_list_t *);
extern int files_open(files_t *, int, mode_t);
extern int files_close(files_t *);
extern int files_seek(files_t *, off_t, int);
extern int files_tell(files_t *, off_t *);
extern int files_read(files_t *, void *, size_t, int *);
extern int files_write(files_t *, void *, size_t, int *);
extern int files_gets(files_t *, char *, int);
extern int files_puts(files_t *, char *);

#endif

