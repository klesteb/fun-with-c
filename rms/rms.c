
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2021 by Kevin L. Esteb                       */
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

#include "rms.h"
#include "when.h"
#include "files.h"
#include "object.h"
#include "tracer.h"
#include "fnm_util.h"
#include "que_util.h"
#include "error_codes.h"

require_klass(OBJECT_KLASS);

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _rms_ctor(object_t *, item_list_t *);
int _rms_dtor(object_t *);
int _rms_compare(rms_t *, rms_t *);
int _rms_override(rms_t *, item_list_t *);

int _rms_init(rms_t *);
int _rms_open(rms_t *);
int _rms_close(rms_t *);
int _rms_unlock(rms_t *);
int _rms_del(rms_t *, off_t);
int _rms_extend(rms_t *, int);
int _rms_lock(rms_t *, off_t);
int _rms_add(rms_t *, void *);
int _rms_record(rms_t *, off_t *);
int _rms_get(rms_t *, off_t, void *);
int _rms_put(rms_t *, off_t, void *);
int _rms_get_sequence(rms_t *, long *);
int _rms_build(rms_t *, void *, void *);
int _rms_next(rms_t *, void *, ssize_t *);
int _rms_prev(rms_t *, void *, ssize_t *);
int _rms_last(rms_t *, void *, ssize_t *);
int _rms_read(rms_t *, void *, ssize_t *);
int _rms_write(rms_t *, void *, ssize_t *);
int _rms_first(rms_t *, void *, ssize_t *);
int _rms_normalize(rms_t *, void *, void *);
int _rms_find(rms_t *, void *, int, int (*compare)(void *, void *), off_t *);
int _rms_search(rms_t *, void *, int, int (*compare)(void *, void *), int (*capture)(rms_t *, void *, queue *), queue *);

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

declare_klass(RMS_KLASS) {
    .size = KLASS_SIZE(rms_t),
    .name = KLASS_NAME(rms_t),
    .ctor = _rms_ctor,
    .dtor = _rms_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

rms_t *rms_create(char *path, char *name, int records, int recsize, int retries, int timeout, tracer_t *dump) {

    int stat = ERR;
    rms_t *self = NULL;
    item_list_t items[8];

    SET_ITEM(items[0], RMS_K_PATH, path, strlen(path), NULL);
    SET_ITEM(items[1], RMS_K_NAME, name, strlen(name), NULL);
    SET_ITEM(items[2], RMS_K_RETRIES, &retries, sizeof(int), NULL);
    SET_ITEM(items[3], RMS_K_TIMEOUT, &timeout, sizeof(int), NULL);
    SET_ITEM(items[4], RMS_K_RECORDS, &records, sizeof(int), NULL);
    SET_ITEM(items[5], RMS_K_RECSIZE, &recsize, sizeof(int), NULL);
    SET_ITEM(items[6], RMS_K_TRACE, dump, 0, NULL);
    SET_ITEM(items[7], 0,0,0,0);

    self = (rms_t *)object_create(RMS_KLASS, items, &stat);

    return self;

}

int rms_destroy(rms_t *self) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            if (object_assert(self, rms_t)) {

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

int rms_override(rms_t *self, item_list_t *items) {

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

int rms_compare(rms_t *us, rms_t *them) {

    int stat = OK;

    when_error {

        if (us != NULL) {

            if (object_assert(them, rms_t)) {

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

char *rms_version(rms_t *self) {

    char *version = VERSION;

    return version;

}

int rms_open(rms_t *self) {

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

int rms_close(rms_t *self) {

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

int rms_add(rms_t *self, void *data) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (data == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_add(self, data);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int rms_del(rms_t *self, off_t recnum) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || ((recnum < 0) && (recnum > self->records))) {

            cause_error(E_INVPARM);

        }

        stat = self->_del(self, recnum);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int rms_get(rms_t *self, off_t recnum, void *data) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (data == NULL) ||
            ((recnum < 0) && (recnum > self->records))) {

            cause_error(E_INVPARM);

        }

        stat = self->_get(self, recnum, data);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int rms_put(rms_t *self, off_t recnum, void *data) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (data == NULL) ||
            ((recnum < 0) && (recnum > self->records))) {

            cause_error(E_INVPARM);

        }

        stat = self->_put(self, recnum, data);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int rms_record(rms_t *self, off_t *recnum) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (recnum == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_record(self, recnum);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int rms_find(rms_t *self, void *data, int len,  int (*compare)(void *, void *), off_t *recnum) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (data == NULL) || (compare == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_find(self, data, len, compare, recnum);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int rms_search(rms_t *self, void *data, int len,  int (*compare)(void *, void *), int (*capture)(rms_t *, void *, queue *), queue *results) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (compare == NULL) || (capture == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_search(self, data, len, compare, capture, results);
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

int _rms_ctor(object_t *object, item_list_t *items) {

    int stat = OK;
    char name[33];
    char seq[256];
    char path[256];
    int timeout = 1;
    int records = 0;
    int recsize = 0;
    char rmsdb[256];
    int retries = 30;
    rms_t *self = NULL;
    tracer_t *dump = NULL;

    if (object != NULL) {

        memset(name, '\0', 33);
        memset(seq, '\0', 256);
        memset(path, '\0', 256);
        memset(rmsdb, '\0', 256);

        /* capture our items */

        if (items != NULL) {

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case RMS_K_PATH: {
                        int len = (items[x].buffer_length > 255) 
                            ? 255 : items[x].buffer_length;
                        memcpy(&path, items[x].buffer_address, len);
                        break;
                    }
                    case RMS_K_NAME: {
                        int len = (items[x].buffer_length > 32) 
                            ? 32 : items[x].buffer_length;
                        memcpy(&name, items[x].buffer_address, len);
                        break;
                    }
                    case RMS_K_RETRIES: {
                        memcpy(&retries, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case RMS_K_TIMEOUT: {
                        memcpy(&timeout, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case RMS_K_RECORDS: {
                        memcpy(&records, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case RMS_K_RECSIZE: {
                        memcpy(&recsize, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case RMS_K_TRACE: {
                        dump = items[x].buffer_address; 
                        break;
                    }
                }

            }

        }

        /* initilize our base klass here */

        object_set_error1(object, OK);

        /* initialize our derived klass here */

        self = RMS(object);

        /* assign our methods here */

        self->ctor = _rms_ctor;
        self->dtor = _rms_dtor;
        self->_compare = _rms_compare;
        self->_override = _rms_override;

        self->_add    = _rms_add;
        self->_del    = _rms_del;
        self->_get    = _rms_get;
        self->_put    = _rms_put;
        self->_init   = _rms_init;
        self->_find   = _rms_find;
        self->_last   = _rms_last;
        self->_lock   = _rms_lock;
        self->_next   = _rms_next;
        self->_open   = _rms_open;
        self->_prev   = _rms_prev;
        self->_read   = _rms_read;
        self->_build  = _rms_build;
        self->_close  = _rms_close;
        self->_first  = _rms_first;
        self->_write  = _rms_write;
        self->_extend = _rms_extend;
        self->_record = _rms_record;
        self->_search = _rms_search;
        self->_unlock = _rms_unlock;
        self->_normalize = _rms_normalize;
        self->_get_sequence = _rms_get_sequence;

        /* initialize internal variables here */

        when_error_in {

            self->record = 0;
            self->trace = dump;
            self->locked = FALSE;
            self->retries = retries;
            self->timeout = timeout;
            self->recsize = recsize;
            self->records = records;
            self->path = strdup(path);

            strncpy(rmsdb, fnm_build(1, FnmPath, name, ".dat", path, NULL), 255);
            self->rmsdb = files_create(rmsdb, retries, timeout);
            check_creation(self->rmsdb);

            strncpy(seq, fnm_build(1, FnmPath, name, ".seq", path, NULL), 255);
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

int _rms_dtor(object_t *object) {

    int stat = OK;
    rms_t *self = RMS(object);

    /* free local resources here */

    files_close(self->rmsdb);
    files_close(self->sequence);
    free(self->path);

    /* walk the chain, freeing as we go */

    object_demote(object, object_t);
    object_destroy(object);

    return stat;

}

int _rms_override(rms_t *self, item_list_t *items) {

    int stat = ERR;

    if (items != NULL) {

        int x;
        for (x = 0;; x++) {

            if ((items[x].buffer_length == 0) &&
                (items[x].item_code == 0)) break;

            switch(items[x].item_code) {
                case RMS_M_DESTRUCTOR: {
                    self->dtor = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case RMS_M_OPEN: {
                    self->_open = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case RMS_M_CLOSE: {
                    self->_close = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case RMS_M_UNLOCK: {
                    self->_unlock = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case RMS_M_LOCK: {
                    self->_lock = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case RMS_M_GET: {
                    self->_get = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case RMS_M_PUT: {
                    self->_put = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case RMS_M_NEXT: {
                    self->_next = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case RMS_M_PREV: {
                    self->_prev = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case RMS_M_LAST: {
                    self->_last = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case RMS_M_WRITE: {
                    self->_write = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case RMS_M_FIRST: {
                    self->_first = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case RMS_M_BUILD: {
                    self->_build = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case RMS_M_EXTEND: {
                    self->_extend = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case RMS_M_ADD: {
                    self->_add = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case RMS_M_DEL: {
                    self->_del = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case RMS_M_NORMALIZE: {
                    self->_normalize = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case RMS_M_FIND: {
                    self->_find = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case RMS_M_SEARCH: {
                    self->_search = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case RMS_M_RECORD: {
                    self->_record = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case RMS_M_INIT: {
                    self->_init = items[x].buffer_address;
                    stat = OK;
                    break;
                }
            }

        }

    }

    return stat;

}

int _rms_compare(rms_t *self, rms_t *other) {

    int stat = ERR;

    if ((object_compare(OBJECT(self), OBJECT(other)) == 0) &&
        (self->ctor == other->ctor) &&
        (self->dtor == other->dtor) &&
        (self->_compare == other->_compare) &&
        (self->_override == other->_override) &&
        (self->_add    == other->_add) &&
        (self->_del    == other->_del) &&
        (self->_get    == other->_get) &&
        (self->_put    == other->_put) &&
        (self->_open   == other->_open) &&
        (self->_lock   == other->_lock) &&
        (self->_next   == other->_next) &&
        (self->_prev   == other->_prev) &&
        (self->_last   == other->_last) &&
        (self->_read   == other->_read) &&
        (self->_write  == other->_write) &&
        (self->_first  == other->_first) &&
        (self->_build  == other->_build) &&
        (self->_close  == other->_close) &&
        (self->_find   == other->_find) &&
        (self->_record == other->_record) &&
        (self->_unlock == other->_unlock) &&
        (self->_extend == other->_extend) &&
        (self->_search == other->_search) &&
        (self->_normalize == other->_normalize) &&
        (self->_get_sequence == other->_get_sequence) &&
        (self->locked == other->locked) &&
        (self->rmsdb == other->rmsdb) &&
        (self->trace  == other->trace) &&
        (self->record == other->record) &&
        (self->trace == other->trace) &&
        (self->locked == other->locked) &&
        (self->retries == other->retries) &&
        (self->timeout == other->timeout) &&
        (self->recsize == other->recsize) &&
        (self->records == other->records) &&
        (strcmp(self->path, other->path) == 0)) {

        stat = OK;

    }

    return stat;

}

int _rms_open(rms_t *self) {

    int stat = OK;
    int exists = 0;
    ssize_t count = 0;
    long sequence = 1;
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

        stat = files_exists(self->rmsdb, &exists);
        check_return(stat, self->rmsdb);

        if (exists) {

            stat = files_open(self->rmsdb, flags, 0);
            check_return(stat, self->rmsdb);

        } else {

            stat = files_open(self->rmsdb, create, mode);
            check_return(stat, self->rmsdb);

            stat = self->_extend(self, self->records);
            check_return(stat, self);

            stat = self->_init(self);
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

int _rms_close(rms_t *self) {

    int stat = OK;

    when_error_in {

        stat = files_close(self->rmsdb);
        check_return(stat, self->rmsdb);

        stat = files_close(self->sequence);
        check_return(stat, self->sequence);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _rms_record(rms_t *self, off_t *recnum) {

    *recnum = self->record;

    return OK;

}

int _rms_first(rms_t *self, void *data, ssize_t *count) {

    int stat = OK;
    void *ondisk = NULL;
    ssize_t position = 0;

    when_error_in {

        errno = 0;
        ondisk = calloc(1, self->recsize);
        if (ondisk == NULL) cause_error(errno);

        stat = files_seek(self->rmsdb, 0, SEEK_SET);
        check_return(stat, self->rmsdb);

        stat = files_tell(self->rmsdb, &position);
        check_return(stat, self->rmsdb);

        self->record = RMS_RECORD(position, self->recsize);

        stat = self->_lock(self, 0);
        check_return(stat, self);

        stat = self->_read(self, ondisk, count);
        check_return(stat, self);

        stat = self->_unlock(self);
        check_return(stat, self);

        if (*count == self->recsize) {

            stat = self->_build(self, ondisk, data);
            check_return(stat, self);

        }

        free(ondisk);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (ondisk) free(ondisk);
        if (self->locked) self->_unlock(self);

    } end_when;

    return stat;

}

int _rms_next(rms_t *self, void *data, ssize_t *count) {

    int stat = OK;
    void *ondisk = NULL;
    ssize_t position = 0;

    when_error_in {

        errno = 0;
        ondisk = calloc(1, self->recsize);
        if (ondisk == NULL) cause_error(errno);

        stat = files_tell(self->rmsdb, &position);
        check_return(stat, self);

        self->record = RMS_RECORD(position, self->recsize);

        stat = self->_lock(self, position);
        check_return(stat, self);

        stat = self->_read(self, ondisk, count);
        check_return(stat, self);

        stat = self->_unlock(self);
        check_return(stat, self);

        if (*count == self->recsize) {

            stat = self->_build(self, ondisk, data);
            check_return(stat, self);

        }

        free(ondisk);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (ondisk) free(ondisk);
        if (self->locked) self->_unlock(self);

    } end_when;

    return stat;

}

int _rms_prev(rms_t *self, void *data, ssize_t *count) {

    int stat = OK;
    void *ondisk = NULL;
    ssize_t position = 0;
    off_t offset = self->recsize;

    when_error_in {

        errno = 0;
        ondisk = calloc(1, self->recsize);
        if (ondisk == NULL) cause_error(errno);

        stat = files_tell(self->rmsdb, &position);
        check_return(stat, self->rmsdb);

        if (position > 0) {

            stat = files_seek(self->rmsdb, -offset, SEEK_CUR);
            check_return(stat, self->rmsdb);

            stat = self->_lock(self, position - offset);
            check_return(stat, self);

            self->record = RMS_RECORD(position - offset, self->recsize);

            stat = self->_read(self, ondisk, count);
            check_return(stat, self);

            stat = self->_unlock(self);
            check_return(stat, self);

            stat = files_seek(self->rmsdb, -offset, SEEK_CUR);
            check_return(stat, self->rmsdb);

            if (*count == self->recsize) {

                stat = self->_build(self, ondisk, data);
                check_return(stat, self);

            }

        } else {

            *count = 0;

        }

        free(ondisk);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (ondisk) free(ondisk);
        if (self->locked) self->_unlock(self);

    } end_when;

    return stat;

}

int _rms_last(rms_t *self, void *data, ssize_t *count) {

    int stat = OK;
    void *ondisk = NULL;
    ssize_t position = 0;
    off_t offset = self->recsize;

    when_error_in {

        errno = 0;
        ondisk = calloc(1, self->recsize);
        if (ondisk == NULL) cause_error(errno);

        stat = files_seek(self->rmsdb, 0, SEEK_END);
        check_return(stat, self->rmsdb);

        stat = files_seek(self->rmsdb, -offset, SEEK_CUR);
        check_return(stat, self->rmsdb);

        stat = files_tell(self->rmsdb, &position);
        check_return(stat, self->rmsdb);

        self->record = RMS_RECORD(position, self->recsize);

        stat = self->_lock(self, position);
        check_return(stat, self);

        stat = self->_read(self, ondisk, count);
        check_return(stat, self);

        stat = self->_unlock(self);
        check_return(stat, self);

        stat = files_seek(self->rmsdb, -offset, SEEK_CUR);
        check_return(stat, self->rmsdb);

        if (*count == self->recsize) {

            stat = self->_build(self, ondisk, data);
            check_return(stat, self);

        }

        free(ondisk);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (ondisk) free(ondisk);
        if (self->locked) self->_unlock(self);

    } end_when;

    return stat;

}

int _rms_get(rms_t *self, off_t recnum, void *record) {

    int stat = OK;
    ssize_t count = 0;
    void *ondisk = NULL;
    off_t offset = RMS_OFFSET(recnum, self->recsize);

    when_error_in {

        errno = 0;
        ondisk = calloc(1, self->recsize);
        if (ondisk == NULL) cause_error(errno);

        stat = files_seek(self->rmsdb, offset, SEEK_SET);
        check_return(stat, self->rmsdb);

        self->_lock(self, offset);
        check_return(stat, self);

        stat = self->_read(self, ondisk, &count);
        check_return(stat, self);

        stat = self->_unlock(self);
        check_return(stat, self);

        if (count != self->recsize) {

            cause_error(EIO);

        }

        stat = self->_build(self, ondisk, record);
        check_return(stat, self);

        free(ondisk);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (ondisk) free(ondisk);
        if (self->locked) self->_unlock(self);

    } end_when;

    return stat;

}

int _rms_put(rms_t *self, off_t recnum, void *record) {

    int stat = OK;
    ssize_t count = 0;
    void *ondisk = NULL;
    ssize_t recsize = self->recsize;
    off_t offset = RMS_OFFSET(recnum, self->recsize);

    when_error_in {

        errno = 0;
        ondisk = calloc(1, self->recsize);
        if (ondisk == NULL) cause_error(errno);

        stat = files_seek(self->rmsdb, offset, SEEK_SET);
        check_return(stat, self->rmsdb);

        stat = self->_lock(self, offset);
        check_return(stat, self);

        stat = self->_read(self, ondisk, &count);
        check_return(stat, self);

        if (count != recsize) {

            cause_error(EIO);

        }

        stat = self->_normalize(self, ondisk, record);
        check_return(stat, self);

        stat = files_seek(self->rmsdb, -recsize, SEEK_CUR);
        check_return(stat, self->rmsdb);

        stat = self->_write(self, record, &count);
        check_return(stat, self);

        stat = self->_unlock(self);
        check_return(stat, self);

        if (count != recsize) {

            cause_error(EIO);

        }

        free(ondisk);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (ondisk) free(ondisk);
        if (self->locked) self->_unlock(self);

    } end_when;

    return stat;

}

int _rms_lock(rms_t *self, off_t offset) {

    int stat = OK;
    int length = self->recsize;

    when_error_in {

        stat = files_lock(self->rmsdb, offset, length);
        check_return(stat, self->rmsdb);

        self->locked = TRUE;

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _rms_unlock(rms_t *self) {

    int stat = OK;

    when_error_in {

        stat = files_unlock(self->rmsdb);
        check_return(stat, self->rmsdb);

        self->locked = FALSE;

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _rms_find(rms_t *self, void *data, int len, int (*compare)(void *, void *), off_t *recnum) {

    int stat = OK;
    ssize_t count = 0;
    void *ondisk = NULL;

    when_error_in {

        *recnum = 0;

        errno = 0;
        ondisk = calloc(1, self->recsize);
        if (ondisk == NULL) cause_error(errno);

        stat = self->_first(self, ondisk, &count);
        check_return(stat, self);

        while (count > 0) {

            if (compare(data, ondisk)) {

                *recnum = self->record;
                break;

            }

            stat = self->_next(self, ondisk, &count);
            check_return(stat, self);

        }

        free(ondisk);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (ondisk) free(ondisk);

    } end_when;

    return stat;

}

int _rms_search(rms_t *self, void *data, int len, int (*compare)(void *, void *), int (*capture)(rms_t *, void *, queue *), queue *results) {

    int stat = OK;
    ssize_t count = 0;
    void *ondisk = NULL;

    when_error_in {

        errno = 0;
        ondisk = calloc(1, self->recsize);
        if (ondisk == NULL) cause_error(errno);

        stat = self->_first(self, ondisk, &count);
        check_return(stat, self);

        while (count > 0) {

            if (compare(data, ondisk)) {

                stat = capture(self, ondisk, results);
                check_return(stat, self);

            }

            stat = self->_next(self, ondisk, &count);
            check_return(stat, self);

        }

        free(ondisk);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (ondisk) free(ondisk);

    } end_when;

    return stat;

}

int _rms_get_sequence(rms_t *self, long *sequence) {

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

int _rms_read(rms_t *self, void *data, ssize_t *count) {

    int stat = OK;

    when_error_in {

        stat = files_read(self->rmsdb, data, self->recsize, count);
        check_return(stat, self->rmsdb);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _rms_write(rms_t *self, void *data, ssize_t *count) {

    int stat = OK;

    when_error_in {

        stat = files_write(self->rmsdb, data, self->recsize, count);
        check_return(stat, self->rmsdb);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _rms_add(rms_t *self, void *record) {

    /* this needs to be overridden */

    return OK;

}

int _rms_build(rms_t *self, void *ondisk, void *data) {

    /* this needs to be overridden */

    return OK;

}
                            
int _rms_del(rms_t *self, off_t recnum) {

    /* this needs to be overriden */

    return OK;
    
}

int _rms_extend(rms_t *self, int amount) {

    /* this needs to be overridden */

    return OK;

}

int _rms_init(rms_t *self) {
    
    /* this may need to be overridden, default is to do nothing */

    return OK;

}

int _rms_normalize(rms_t *self, void *ondisk, void *data) {

    /* this needs to be overridden */

    return OK;

}

