
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2020 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that this copyright notice appears in all copies. The author    */
/*  makes no representations about the suitability of this software for      */
/*  any purpose. It is provided "as is" without express or implied warranty. */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#include <stdio.h>
#include <errno.h>

#include "msgs.h"
#include "room.h"
#include "when.h"
#include "files.h"
#include "handler.h"
#include "object.h"
#include "fnm_util.h"
#include "error_codes.h"

require_klass(OBJECT_KLASS);

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _room_ctor(object_t *, item_list_t *);
int _room_dtor(object_t *);

int _room_compare(room_t *, room_t *);
int _room_override(room_t *, item_list_t *);

int _room_open(room_t *);
int _room_close(room_t *);
int _room_del(room_t *, int);
int _room_unlock(room_t *self);
int _room_lock(room_t *, off_t);
int _room_extend(room_t *, int);
int _room_add(room_t *, room_base_t *);
int _room_get_sequence(room_t *, long *);
int _room_get(room_t *, int, room_base_t *);
int _room_put(room_t *, int, room_base_t *);
int _room_read(room_t *, room_base_t *, ssize_t *);
int _room_next(room_t *, room_base_t *, ssize_t *);
int _room_prev(room_t *, room_base_t *, ssize_t *);
int _room_last(room_t *, room_base_t *, ssize_t *);
int _room_first(room_t *, room_base_t *, ssize_t *);
int _room_write(room_t *, room_base_t *, ssize_t *);
int _room_build(room_t *, room_base_t *, room_base_t *);
int _room_get_handler(room_t *, room_base_t *, void **);
int _room_normalize(room_t *, room_base_t *, room_base_t *);
int _room_find(room_t *, void *, int, int (*compare)(void *, int, room_base_t *), int *);
int _room_search(room_t *, void *, int, int (*compare)(void *, int, room_base_t *), queue *);

/*----------------------------------------------------------------*/
/* private klass methods                                          */
/*----------------------------------------------------------------*/

static int _init_defaults(room_t *);
static int _detach_handler(room_t *);
static int _remove_handler(room_t *);
static int _attach_handler(room_t *, room_base_t *);

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

declare_klass(ROOM_KLASS) {
    .size = KLASS_SIZE(room_t),
    .name = KLASS_NAME(room_t),
    .ctor = _room_ctor,
    .dtor = _room_dtor,
};

/*----------------------------------------------------------------*/
/* klass macros                                                   */
/*----------------------------------------------------------------*/

#define ROOM_OFFSET(n)   ((((n) - 1) * sizeof(room_base_t)))
#define ROOM_RECORD(n)   (((n) / sizeof(room_base_t)) + 1)

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

room_t *room_create(char *dbpath, char *msgpath, int rooms, int retries, int timeout, int base, tracer_t *dump) {

    int stat = ERR;
    room_t *self = NULL;
    item_list_t items[7];

    SET_ITEM(items[0], ROOM_K_MSGBASE, msgpath, strlen(msgpath), NULL);
    SET_ITEM(items[1], ROOM_K_DATABASE, dbpath, strlen(dbpath), NULL);
    SET_ITEM(items[2], ROOM_K_RETRIES, &retries, sizeof(int), NULL);
    SET_ITEM(items[3], ROOM_K_TIMEOUT, &timeout, sizeof(int), NULL);
    SET_ITEM(items[4], ROOM_K_BASE, &base, sizeof(int), NULL);
    SET_ITEM(items[4], ROOM_K_ROOMS, &rooms, sizeof(int), NULL);
    SET_ITEM(items[5], ROOM_K_TRACE, dump, 0, NULL);
    SET_ITEM(items[6], 0,0,0,0);

    self = (room_t *)object_create(ROOM_KLASS, items, &stat);

    return self;

}

int room_destroy(room_t *self) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            if (object_assert(self, room_t)) {

                stat = self->dtor(OBJECT(self));
                check_status(stat, OK, E_INVOPS);

            } else {

                cause_error(E_INVOBJ);

            }

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int room_override(room_t *self, item_list_t *items) {

    int stat = OK;

    when_error {
        
        if (self != NULL) {

            stat = self->_override(self, items);
            check_status(stat, OK, E_INVOPS);

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int room_compare(room_t *us, room_t *them) {

    int stat = OK;

    when_error {

        if (us != NULL) {

            if (object_assert(them, room_t)) {

                stat = us->_compare(us, them);
                check_status(stat, OK, E_NOTSAME);

            } else {

                cause_error(E_INVOBJ);

            }

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(us);

    } end_when;

    return stat;

}

char *room_version(room_t *self) {

    char *version = VERSION;

    return version;

}

int room_open(room_t *self) {

    int stat = OK;

    when_error_in {

        if (self == NULL) {

            cause_error(E_INVPARM);

        }

        stat = self->_open(self);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int room_close(room_t *self) {

    int stat = OK;

    when_error_in {

        if (self == NULL) {

            cause_error(E_INVPARM);

        }

        stat = self->_close(self);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int room_add(room_t *self, room_base_t *room) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (room == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_add(self, room);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int room_del(room_t *self, int index) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) ||
            ((index < 0) && (index > self->rooms))) {

            cause_error(E_INVPARM);

        }

        stat = self->_del(self, index);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int room_get(room_t *self, int index, room_base_t *room) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (room == NULL) ||
            ((index < 0) && (index > self->rooms))) {

            cause_error(E_INVPARM);

        }

        stat = self->_get(self, index, room);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int room_put(room_t *self, int index, room_base_t *room) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (room == NULL) ||
            ((index < 0) && (index > self->rooms))) {

            cause_error(E_INVPARM);

        }

        stat = self->_put(self, index, room);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int room_extend(room_t *self, int amount) {

    int stat = OK;

    when_error_in {

        if (self == NULL) {

            cause_error(E_INVPARM);

        }

        stat = self->_extend(self, amount);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int room_find(room_t *self, void *data, int len, int (*compare)(void *, int, room_base_t *), int *index) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (data == NULL) || (compare == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_find(self, data, len, compare, index);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int room_search(room_t *self, void *data, int len, int (*compare)(void *, int, room_base_t *), queue *results) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (compare == NULL) || (results == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_search(self, data, len, compare, results);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int room_handler(room_t *self, room_base_t *room, void **handle) {
    
    int stat = OK;

    when_error_in {

        if ((self == NULL) || (room == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_get_handler(self, room, handle);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

int _room_ctor(object_t *object, item_list_t *items) {

    int base = 0;
    char seq[256];
    int stat = ERR;
    int rooms = 32;
    int timeout = 1;
    int retries = 30;
    char msgbase[256];
    char database[256];
    char roombase[256];
    room_t *self = NULL;
    tracer_t *dump = NULL;

    if (object != NULL) {

        memset(seq, '\0', 256);
        memset(msgbase, '\0', 256);
        memset(database, '\0', 256);
        memset(roombase, '\0', 256);

        strcpy(msgbase, ".");
        strcpy(database, ".");

        /* capture our items */

        if (items != NULL) {

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case ROOM_K_MSGBASE: {
                        memcpy(&msgbase, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case ROOM_K_DATABASE: {
                        memcpy(&database, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case ROOM_K_RETRIES: {
                        memcpy(&retries, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case ROOM_K_TIMEOUT: {
                        memcpy(&timeout, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case ROOM_K_TRACE: {
                        dump = items[x].buffer_address; 
                        break;
                    }
                    case ROOM_K_BASE: {
                        memcpy(&base, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        if (base < 1) base = 1;
                        break;
                    }
                    case ROOM_K_ROOMS: {
                        memcpy(&rooms, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                }

            }

        }

        /* initilize our base klass here */

        object_set_error1(object, OK);

        /* initialize our derived klass here */

        self = ROOM(object);

        /* assign our methods here */

        self->ctor = _room_ctor;
        self->dtor = _room_dtor;
        self->_compare = _room_compare;
        self->_override = _room_override;

        self->_open  = _room_open;
        self->_close = _room_close;
        self->_add   = _room_add;
        self->_next  = _room_next;
        self->_prev  = _room_prev;
        self->_last  = _room_last;
        self->_first = _room_first;
        self->_del   = _room_del;
        self->_get   = _room_get;
        self->_put   = _room_put;
        self->_read  = _room_read;
        self->_write = _room_write;
        self->_build = _room_build;
        self->_lock  = _room_lock;
        self->_find  = _room_find;
        self->_unlock = _room_unlock;
        self->_extend = _room_extend;
        self->_search = _room_search;
        self->_normalize = _room_normalize;
        self->_get_handler = _room_get_handler;
        self->_get_sequence = _room_get_sequence;

        /* initialize internal variables here */

        when_error_in {

            self->index = 0;
            self->base = base;
            self->trace = dump;
            self->rooms = rooms;
            self->handler = NULL;
            self->locked = FALSE;
            self->retries = retries;
            self->timeout = timeout;
            self->path = strdup(msgbase);

            strncpy(roombase, fnm_build(1, FnmPath, "rooms", ".dat", database, NULL), 255);
            self->roomdb = files_create(roombase, retries, timeout);
            check_creation(self->roomdb);

            strncpy(seq, fnm_build(1, FnmPath, "rooms", ".seq", database, NULL), 255);
            self->sequence = files_create(seq, retries, timeout);
            check_creation(self->sequence);

            exit_when;

        } use {

            stat = ERR;
            process_error(self);

        } end_when;

    }

    return stat;

}

int _room_dtor(object_t *object) {

    int stat = OK;
    room_t *self = ROOM(object);

    /* free local resources here */

    _detach_handler(self);

    free(self->path);
    files_close(self->roomdb);
    files_close(self->sequence);

    /* walk the chain, freeing as we go */

    object_demote(object, object_t);
    object_destroy(object);

    return stat;

}

int _room_override(room_t *self, item_list_t *items) {

    int stat = ERR;

    if (items != NULL) {

        int x;
        for (x = 0;; x++) {

            if ((items[x].buffer_length == 0) &&
                (items[x].item_code == 0)) break;

            switch(items[x].item_code) {
                case ROOM_M_DESTRUCTOR: {
                    self->dtor = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case ROOM_M_OPEN: {
                    self->_open = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case ROOM_M_CLOSE: {
                    self->_close = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case ROOM_M_DEL: {
                    self->_del = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case ROOM_M_SIZE: {
                    self->_size = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case ROOM_M_ADD: {
                    self->_add = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case ROOM_M_GET: {
                    self->_get = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case ROOM_M_PUT: {
                    self->_put = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case ROOM_M_READ: {
                    self->_read = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case ROOM_M_NEXT: {
                    self->_next = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case ROOM_M_PREV: {
                    self->_prev = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case ROOM_M_LAST: {
                    self->_last = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case ROOM_M_FIRST: {
                    self->_first = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case ROOM_M_WRITE: {
                    self->_write = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case ROOM_M_BUILD: {
                    self->_build = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case ROOM_M_LOCK: {
                    self->_lock = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case ROOM_M_UNLOCK: {
                    self->_unlock = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case ROOM_M_EXTEND: {
                    self->_extend = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case ROOM_M_NORMALIZE: {
                    self->_normalize = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case ROOM_M_FIND: {
                    self->_find = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case ROOM_M_SEARCH: {
                    self->_search = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case ROOM_M_GET_HANDLER: {
                    self->_get_handler = items[x].buffer_address;
                    stat = OK;
                    break;
                }
            }

        }

    }

    return stat;

}

int _room_compare(room_t *self, room_t *other) {

    int stat = ERR;

    if ((object_compare(OBJECT(self), OBJECT(other)) == 0) &&
        (self->ctor == other->ctor) &&
        (self->dtor == other->dtor) &&
        (self->_compare == other->_compare) &&
        (self->_override == other->_override) &&
        (self->_open  == other->_open) &&
        (self->_close == other->_close) &&
        (self->_add   == other->_add) &&
        (self->_next  == other->_next) &&
        (self->_prev  == other->_prev) &&
        (self->_last  == other->_last) &&
        (self->_first == other->_first) &&
        (self->_del   == other->_del) &&
        (self->_get   == other->_get) &&
        (self->_put   == other->_put) &&
        (self->_read  == other->_read) &&
        (self->_write == other->_write) &&
        (self->_lock == other->_lock) &&
        (self->_unlock == other->_unlock) &&
        (self->_extend == other->_extend) &&
        (self->_normalize == other->_normalize) &&
        (self->_find == other->_find) &&
        (self->_search == other->_search) &&
        (self->base == other->base) &&
        (self->retries == other->retries) &&
        (self->timeout == other->timeout) &&
        (self->path == other->path) &&
        (self->handler == other->handler) &&
        (self->rooms = other->rooms) &&
        (self->roomdb = other->roomdb) &&
        (self->sequence = other->sequence) &&
        (self->trace == other->trace)) {

        stat = OK;

    }

    return stat;

}

int _room_open(room_t *self) {

    int stat = OK;
    int exists = 0;
    long sequence = 1;
    ssize_t count = 0;
    int flags = O_RDWR;
    int mode = (S_IRWXU | S_IRWXG);
    int create = (O_RDWR | O_CREAT);

    when_error_in {

        stat = files_exists(self->sequence, &exists);
        check_return(stat, self->sequence);

        if (exists) {

            stat = files_open(self->sequence, flags, 0);
            check_return(stat, self->sequence);

        } else {

            stat = files_open(self->sequence, create, mode);
            check_return(stat, self->sequence);

            stat = files_write(self->sequence, &sequence, sizeof(long), &count);
            check_return(stat, self->sequence);

            if (count != sizeof(long)) {

                cause_error(EIO);

            }

        }

        stat = files_exists(self->roomdb, &exists);
        check_return(stat, self->roomdb);

        if (exists) {

            stat = files_open(self->roomdb, flags, 0);
            check_return(stat, self->roomdb);

        } else {

            stat = files_open(self->roomdb, create, mode);
            check_return(stat, self->roomdb);

            stat = self->_extend(self, self->rooms);
            check_return(stat, self);

            stat = _init_defaults(self);
            check_return(stat, self);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _room_close(room_t *self) {

    int stat = OK;

    when_error_in {

        stat = _detach_handler(self);
        check_return(stat, self);
        
        stat = files_close(self->roomdb);
        check_return(stat, self->roomdb);

        stat = files_close(self->sequence);
        check_return(stat, self->sequence);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _room_first(room_t *self, room_base_t *room, ssize_t *count) {

    int stat = OK;
    room_base_t ondisk;
    ssize_t position = 0;

    when_error_in {

        stat = files_seek(self->roomdb, 0, SEEK_SET);
        check_return(stat, self->roomdb);

        stat = files_tell(self->roomdb, &position);
        check_return(stat, self->roomdb);

        self->index = ROOM_RECORD(position);

        stat = self->_lock(self, 0);
        check_return(stat, self);

        stat = self->_read(self, &ondisk, count);
        check_return(stat, self);

        stat = self->_unlock(self);
        check_return(stat, self);

        if (*count == sizeof(room_base_t)) {

            stat = self->_build(self, &ondisk, room);
            check_return(stat, self);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (self->locked) self->_unlock(self);

    } end_when;

    return stat;

}

int _room_next(room_t *self, room_base_t *room, ssize_t *count) {

    int stat = OK;
    room_base_t ondisk;
    ssize_t position = 0;

    when_error_in {

        stat = files_tell(self->roomdb, &position);
        check_return(stat, self);

        self->index = ROOM_RECORD(position);

        stat = self->_lock(self, position);
        check_return(stat, self);

        stat = self->_read(self, &ondisk, count);
        check_return(stat, self);

        stat = self->_unlock(self);
        check_return(stat, self);

        if (*count == sizeof(room_base_t )) {

            stat = self->_build(self, &ondisk, room);
            check_return(stat, self);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (self->locked) self->_unlock(self);

    } end_when;

    return stat;

}

int _room_prev(room_t *self, room_base_t *room, ssize_t *count) {

    int stat = OK;
    room_base_t ondisk;
    ssize_t position = 0;
    off_t offset = sizeof(room_base_t);

    when_error_in {

        stat = files_tell(self->roomdb, &position);
        check_return(stat, self->roomdb);

        if (position > 0) {

            stat = files_seek(self->roomdb, -offset, SEEK_CUR);
            check_return(stat, self->roomdb);

            stat = self->_lock(self, position - offset);
            check_return(stat, self);

            self->index = ROOM_RECORD(position - offset);

            stat = self->_read(self, &ondisk, count);
            check_return(stat, self);

            stat = self->_unlock(self);
            check_return(stat, self);

            stat = files_seek(self->roomdb, -offset, SEEK_CUR);
            check_return(stat, self->roomdb);

            if (*count == sizeof(room_base_t)) {

                stat = self->_build(self, &ondisk, room);
                check_return(stat, self);

            }

        } else {

            *count = 0;

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (self->locked) self->_unlock(self);

    } end_when;

    return stat;

}

int _room_last(room_t *self, room_base_t *room, ssize_t *count) {

    int stat = OK;
    room_base_t ondisk;
    ssize_t position = 0;
    off_t offset = sizeof(room_base_t);

    when_error_in {

        stat = files_seek(self->roomdb, 0, SEEK_END);
        check_return(stat, self->roomdb);

        stat = files_seek(self->roomdb, -offset, SEEK_CUR);
        check_return(stat, self->roomdb);

        stat = files_tell(self->roomdb, &position);
        check_return(stat, self->roomdb);

        self->index = ROOM_RECORD(position);

        stat = self->_lock(self, position);
        check_return(stat, self);

        stat = self->_read(self, &ondisk, count);
        check_return(stat, self);

        stat = self->_unlock(self);
        check_return(stat, self);

        stat = files_seek(self->roomdb, -offset, SEEK_CUR);
        check_return(stat, self->roomdb);

        if (*count == sizeof(room_base_t)) {

            stat = self->_build(self, &ondisk, room);
            check_return(stat, self);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (self->locked) self->_unlock(self);

    } end_when;

    return stat;

}

int _room_add(room_t *self, room_base_t *room) {

    int stat = OK;
    ssize_t count = 0;
    room_base_t ondisk;
    int created = FALSE;
    ssize_t position = 0;
    ssize_t recsize = sizeof(room_base_t);

    when_error_in {

        stat = self->_first(self, &ondisk, &count);
        check_return(stat, self);

        while (count > 0) {

            if ((ondisk.flags == 0) && (ondisk.conference == 0)) {

                room->roomnum = ondisk.roomnum;
                room->revision = ondisk.revision + 1;

                stat = files_seek(self->roomdb, -recsize, SEEK_CUR);
                check_return(stat, self->roomdb);

                stat = files_tell(self->roomdb, &position);
                check_return(stat, self->roomdb);

                stat = self->_lock(self, position);
                check_return(stat, self);

                stat = self->_write(self, room, &count);
                check_return(stat, self);

                stat = _attach_handler(self, room);
                check_return(stat, self);

                stat = self->_unlock(self);
                check_return(stat, self);

                if (count != sizeof(room_base_t)) {

                    cause_error(EIO);

                }

                created = TRUE;
                break;

            }

            stat = self->_next(self, &ondisk, &count);
            check_return(stat, self);

        }

        if (created == FALSE) {

            cause_error(EOVERFLOW);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (self->locked) self->_unlock(self);

    } end_when;

    return stat;

}

int _room_del(room_t *self, int index) {

    int stat = OK;
    ssize_t count = 0;
    room_base_t ondisk;
    off_t offset = ROOM_OFFSET(index);
    off_t recsize = sizeof(room_base_t);

    when_error_in {

        stat = files_seek(self->roomdb, offset, SEEK_SET);
        check_return(stat, self->roomdb);

        stat = self->_lock(self, offset);
        check_return(stat, self);

        stat = self->_read(self, &ondisk, &count);
        check_return(stat, self);

        if (! (ondisk.flags & RM_PERMROOM)) {

            ondisk.aide = 0;
            ondisk.flags = 0;
            ondisk.revision++;
            ondisk.conference = 0;
            memset(&ondisk.name, '\0', 32);

            stat = files_seek(self->roomdb, -recsize, SEEK_CUR);
            check_return(stat, self->roomdb);

            stat = self->_write(self, &ondisk, &count);
            check_return(stat, self);

            stat = _remove_handler(self);
            check_return(stat, self);

        }

        stat = self->_unlock(self);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (self->locked) self->_unlock(self);

    } end_when;

    return stat;

}

int _room_get(room_t *self, int index, room_base_t *room) {

    int stat = OK;
    ssize_t count = 0;
    room_base_t ondisk;
    off_t offset = ROOM_OFFSET(index);

    when_error_in {

        stat = files_seek(self->roomdb, offset, SEEK_SET);
        check_return(stat, self->roomdb);

        self->_lock(self, offset);
        check_return(stat, self);

        stat = self->_read(self, &ondisk, &count);
        check_return(stat, self);

        stat = self->_unlock(self);
        check_return(stat, self);

        if (count == sizeof(room_base_t)) {

            stat = self->_build(self, &ondisk, room);
            check_return(stat, self);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (self->locked) self->_unlock(self);

    } end_when;

    return stat;

}

int _room_put(room_t *self, int index, room_base_t *room) {

    int stat = OK;
    ssize_t count = 0;
    room_base_t ondisk;
    off_t offset = ROOM_OFFSET(index);
    off_t recsize = sizeof(room_base_t);

    when_error_in {

        stat = files_seek(self->roomdb, offset, SEEK_SET);
        check_return(stat, self->roomdb);

        stat = self->_lock(self, offset);
        check_return(stat, self);

        stat = self->_read(self, &ondisk, &count);
        check_return(stat, self);

        if (ondisk.revision > room->revision) {

            self->_normalize(self, &ondisk, room);
            check_return(stat, self);

        } else {

            room->revision = ondisk.revision + 1;;

        }

        stat = files_seek(self->roomdb, -recsize, SEEK_CUR);
        check_return(stat, self->roomdb);

        stat = self->_write(self, room, &count);
        check_return(stat, self);

        stat = self->_unlock(self);
        check_return(stat, self);

        if (count != sizeof(room_base_t)) {

            cause_error(EIO);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (self->locked) self->_unlock(self);

    } end_when;

    return stat;

}

int _room_lock(room_t *self, off_t offset) {

    int stat = OK;
    int length = sizeof(room_base_t);

    when_error_in {

        stat = files_lock(self->roomdb, offset, length);
        check_return(stat, self->roomdb);

        self->locked = TRUE;

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _room_unlock(room_t *self) {

    int stat = OK;

    when_error_in {

        stat = files_unlock(self->roomdb);
        check_return(stat, self->roomdb);

        self->locked = FALSE;

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _room_get_handler(room_t *self, room_base_t *room, void **handle) {

    int stat = OK;
    handler_t *handler = (handler_t *)self->handler;

    when_error_in {

        if (handler != NULL) {

            stat = handler_handle(handler, handle);
            check_return(stat, handler);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _room_get_sequence(room_t *self, long *sequence) {

    int stat = OK;
    long buffer = 0;
    ssize_t count = 0;
    int locked = FALSE;

    when_error_in {

        stat = files_seek(self->sequence, 0, SEEK_SET);
        check_return(stat, self->sequence);

        stat = files_lock(self->sequence, 0, sizeof(long));
        check_return(stat, self->sequence);

        locked = TRUE;

        stat = files_read(self->sequence, &buffer, sizeof(long), &count);
        check_return(stat, self->sequence);

        if (count != sizeof(long)) {

            cause_error(EIO);

        }

        *sequence = buffer;
        buffer++;

        stat = files_seek(self->sequence, 0, SEEK_SET);
        check_return(stat, self->sequence);

        stat = files_write(self->sequence, &buffer, sizeof(long), &count);
        check_return(stat, self->sequence);

        if (count != sizeof(long)) {

            cause_error(EIO);

        }

        stat = files_unlock(self->sequence);
        check_return(stat, self->sequence);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (locked) files_unlock(self->sequence);

    } end_when;

    return stat;

}

int _room_find(room_t *self, void *data, int len, int (*compare)(void *, int, room_base_t *), int *index) {

    int stat = OK;
    ssize_t count = 0;
    room_base_t ondisk;

    when_error_in {

        *index = 0;

        stat = self->_first(self, &ondisk, &count);
        check_return(stat, self);

        while (count > 0) {

            if (compare(data, len, &ondisk)) {

                *index = self->index;
                break;

            }

            stat = self->_next(self, &ondisk, &count);
            check_return(stat, self);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _room_search(room_t *self, void *data, int len, int (*compare)(void *, int, room_base_t *), queue *results) {

    int stat = OK;
    ssize_t count = 0;
    room_base_t ondisk;
    room_search_t *result = NULL;

    when_error_in {

        stat = self->_first(self, &ondisk, &count);
        check_return(stat, self);

        while (count > 0) {

            if (compare(data, len, &ondisk)) {

                errno = 0;
                result = calloc(1, sizeof(room_search_t));
                if (result == NULL) {

                    cause_error(errno);

                }

                strcpy(result->name, ondisk.name);
                result->index = self->index;

                stat = que_push_head(results, result);
                check_status(stat, QUE_OK, E_NOQUEUE);

            }

            stat = self->_next(self, &ondisk, &count);
            check_return(stat, self);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _room_extend(room_t *self, int amount) {

    int stat = OK;
    room_base_t room;
    int revision = 1;
    long sequence = 0;
    ssize_t count = 0;
    ssize_t position = 0;

    when_error_in {

        memset(&room, '\0', sizeof(room_base_t));

        room.aide = 0;
        room.flags = 0;
        room.conference = 0;
        room.base = self->base;
        room.revision = revision;
        room.retries = self->retries;
        room.timeout = self->timeout;
        strncpy(room.path, fnm_build(1, FnmPath, self->path, NULL), 255);

        stat = files_seek(self->roomdb, 0, SEEK_END);
        check_return(stat, self->roomdb);

        int x;
        for (x = 0; x < amount; x++) {

            stat = self->_get_sequence(self, &sequence);
            check_return(stat, self);

            room.roomnum = sequence;

            stat = files_tell(self->roomdb, &position);
            check_return(stat, self->roomdb);

            stat = self->_lock(self, position);
            check_return(stat, self);

            stat = self->_write(self, &room, &count);
            check_return(stat, self);

            stat = self->_unlock(self);
            check_return(stat, self);

            if (count != sizeof(room_base_t)) {

                cause_error(EIO);

            }

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _room_normalize(room_t *self, room_base_t *ondisk, room_base_t *room) {

    (*room).aide = ondisk->aide;
    (*room).base = ondisk->base;
    (*room).flags = ondisk->flags;
    (*room).roomnum = ondisk->roomnum;
    (*room).retries = ondisk->retries;
    (*room).timeout = ondisk->timeout;
    (*room).revision = ondisk->revision + 1;
    (*room).conference = ondisk->conference;

    memset((*room).name, '\0', 32);
    strncpy((*room).name, ondisk->name, 31);

    memset((*room).path, '\0', 256);
    strncpy((*room).path, ondisk->path, 255);

    return OK;

}

int _room_build(room_t *self, room_base_t *ondisk, room_base_t *room) {

    int stat = OK;

    when_error_in {

        (*room).aide = ondisk->aide;
        (*room).base = ondisk->base;
        (*room).flags = ondisk->flags;
        (*room).roomnum = ondisk->roomnum;
        (*room).retries = ondisk->retries;
        (*room).timeout = ondisk->timeout;
        (*room).revision = ondisk->revision;
        (*room).conference = ondisk->conference;

        memset((*room).name, '\0', 32);
        strncpy((*room).name, ondisk->name, 31);

        memset((*room).path, '\0', 256);
        strncpy((*room).path, ondisk->path, 255);

        stat = _detach_handler(self);
        check_return(stat, self);

        stat = _attach_handler(self, room);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _room_read(room_t *self, room_base_t *room, ssize_t *count) {

    int stat = OK;

    when_error_in {

        stat = files_read(self->roomdb, room, sizeof(room_base_t), count);
        check_return(stat, self->roomdb);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _room_write(room_t *self, room_base_t *room, ssize_t *count) {

    int stat = OK;

    when_error_in {

        stat = files_write(self->roomdb, room, sizeof(room_base_t), count);
        check_return(stat, self->roomdb);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

/*----------------------------------------------------------------*/
/* private methods                                                */
/*----------------------------------------------------------------*/

static int _init_defaults(room_t *self) {

    int stat = OK;
    handler_t *handler = NULL;

    when_error_in {

        /* create default message rooms */

        handler = msgs_create(self->roomdb, self->path, self->retries, self->timeout, self->base, self->trace);
        check_creation(handler);

        stat = handler_init(handler);
        check_return(stat, handler);

        stat = handler_destroy(handler);
        check_return(stat, handler);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

static int _attach_handler(room_t *self, room_base_t *room) {

    int stat = OK;
    handler_t *handler = (handler_t *)self->handler;

    when_error_in {

        if (handler == NULL) {

            if (room->flags & RM_MESSAGES) {

                handler = msgs_create(self->roomdb, room->path, room->retries, room->timeout, room->base, self->trace);
                check_creation(handler);

            } else if (room->flags & RM_BULLETIN) {
                
            } else if (room->flags & RM_DIRECTORY) {
                
            } else if (room->flags & RM_SUBSYS) {
                
            }

        }

        if (handler != NULL) {

            stat = handler_attach(handler, room);
            check_return(stat, handler);

        }

        self->handler = (void *)handler;

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

static int _remove_handler(room_t *self) {

    int stat = OK;
    handler_t *handler = (handler_t *)self->handler;

    when_error_in {

        if (handler != NULL) {

            stat = handler_remove(handler);
            check_return(stat, handler);

            stat = _detach_handler(self);
            check_return(stat, handler);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

static int _detach_handler(room_t *self) {
    
    int stat = OK;
    handler_t *handler = (handler_t *)self->handler;

    when_error_in {

        if (handler != NULL) {

            stat = handler_detach(handler);
            check_return(stat, handler);

            stat = handler_destroy(handler);
            check_return(stat, handler);

            (*self).handler = NULL;

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

