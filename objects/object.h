
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

/*---------------------------------------------------------------------------*/
/* the concept for these routines was extracted from vk_object.[c|h]         */
/* located at: https://github.com/TragicWarrior/libviper                     */
/*---------------------------------------------------------------------------*/

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <string.h>
#include <stdlib.h>

#include "status.h"
#include "error_trace.h"
#include "item_list.h"

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

typedef struct _object_s object_t;

struct _object_s {
    size_t        size;
    const char    *name;
    error_trace_t *error;
    int          (*ctor)(object_t *, item_list_t *);
    int          (*dtor)(object_t *);
};

/*----------------------------------------------------------------*/
/* constants                                                      */
/*----------------------------------------------------------------*/

#define KLASS_SIZE(klass_size)  sizeof(klass_size)
#define KLASS_NAME(klass_name)  #klass_name

#define declare_klass(klass) object_t *klass = &(object_t)
#define require_klass(klass) extern object_t *klass

#define OBJECT(x) ((object_t *)(x))

#define object_create(klass, items, stat) object_construct(klass, items, stat)

#define object_assert(klass, type) \
            _klass_assert((char *)OBJECT(klass)->name, \
            (char *) #type, OBJECT(klass)->size, \
            sizeof(type))

#define object_demote(object, type) \
            _klass_demote(OBJECT(object), \
            (char *) #type, sizeof(type))

#define object_clone(klass) \
            _klass_clone((klass))

#define object_set_error1(object, error) \
            object_set_error(OBJECT((object)), (error), __LINE__, __FILE__, __func__)

#define object_set_error2(object, error, lineno, file, func) \
            object_set_error(OBJECT((object)), (error), (lineno), (file), (func))

/*----------------------------------------------------------------*/
/* interface                                                      */
/*----------------------------------------------------------------*/

extern object_t *object_construct(const void *, item_list_t *, int *);
extern int object_destroy(object_t *);
extern int object_compare(object_t *, object_t *);
extern int object_get_error(object_t *, error_trace_t *);
extern int object_set_error(object_t *self, int, int, char *, const char *);

static inline int _klass_assert(char *kn1, char *kn2, int ks1, int ks2) {

    if ((strcmp(kn1, kn2) != 0) || (ks1 != ks2)) return 0;

    return 1;

}

static inline void _klass_demote(object_t *object, char *kn, int ks) {

    object->name = kn;
    object->size = ks;

    return;

}

static inline object_t *_klass_clone(void *klass) {

	/* this is a shallow clone */

	object_t *temp = NULL;
	int size = OBJECT(klass)->size;

	if ((temp = calloc(1, size)) != NULL) {

		memcpy(temp, klass, size);

	}

	return temp;

}

#endif

