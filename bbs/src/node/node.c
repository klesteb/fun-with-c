
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

#include "node.h"
#include "when.h"
#include "files.h"
#include "object.h"
#include "tracer.h"
#include "fnm_util.h"
#include "error_codes.h"

require_klass(OBJECT_KLASS);

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _node_ctor(object_t *, item_list_t *);
int _node_dtor(object_t *);
int _node_compare(node_t *, node_t *);
int _node_override(node_t *, item_list_t *);

int _node_open(node_t *);
int _node_close(node_t *);
int _node_unlock(node_t *);
int _node_lock(node_t *, off_t);
int _node_extend(node_t *, int);
int _node_get(node_t *, int, node_base_t *);
int _node_put(node_t *, int, node_base_t *);
int _node_get_message(node_t *, long, char **);
int _node_put_message(node_t *, char *, long *);
int _node_next(node_t *, node_base_t *, ssize_t *);
int _node_prev(node_t *, node_base_t *, ssize_t *);
int _node_last(node_t *, node_base_t *, ssize_t *);
int _node_read(node_t *, node_base_t *, ssize_t *);
int _node_get_sequence(node_t *, files_t *, long *);
int _node_write(node_t *, node_base_t *, ssize_t *);
int _node_first(node_t *, node_base_t *, ssize_t *);
int _node_build(node_t *, node_base_t *, node_base_t *);
int _node_normalize(node_t *, node_base_t *, node_base_t *);
int _node_find(node_t *self, void *data, int len, int (*compare)(void *, int, node_base_t *), int *index);
int _node_search(node_t *self, void *data, int len, int (*compare)(void *, int, node_base_t *), queue *results);

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

declare_klass(NODE_KLASS) {
    .size = KLASS_SIZE(node_t),
    .name = KLASS_NAME(node_t),
    .ctor = _node_ctor,
    .dtor = _node_dtor,
};

/*----------------------------------------------------------------*/
/* klass macros                                                   */
/*----------------------------------------------------------------*/

#define NODE_OFFSET(n)   ((((n) - 1) * sizeof(node_base_t)))
#define NODE_RECORD(n)   (((n) / sizeof(node_base_t)) + 1)

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

node_t *node_create(char *path, int nodes, int retries, int timeout, tracer_t *dump) {

    int stat = ERR;
    node_t *self = NULL;
    item_list_t items[6];

    SET_ITEM(items[0], NODE_K_PATH, path, strlen(path), NULL);
    SET_ITEM(items[1], NODE_K_NODES, &nodes, sizeof(int), NULL);
    SET_ITEM(items[2], NODE_K_RETRIES, &retries, sizeof(int), NULL);
    SET_ITEM(items[3], NODE_K_TIMEOUT, &timeout, sizeof(int), NULL);
    SET_ITEM(items[4], NODE_K_TRACE, dump, 0, NULL);
    SET_ITEM(items[5], 0,0,0,0);

    self = (node_t *)object_create(NODE_KLASS, items, &stat);

    return self;

}

int node_destroy(node_t *self) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            if (object_assert(self, node_t)) {

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

int node_override(node_t *self, item_list_t *items) {

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

int node_compare(node_t *us, node_t *them) {

    int stat = OK;

    when_error {

        if (us != NULL) {

            if (object_assert(them, node_t)) {

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

char *node_version(node_t *self) {

    char *version = VERSION;

    return version;

}

int node_open(node_t *self) {

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

int node_close(node_t *self) {

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

int node_get(node_t *self, int index, node_base_t *node) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (node == NULL) ||
            ((index < 0) && (index > self->nodes))) {

            cause_error(E_INVPARM);

        }

        stat = self->_get(self, index, node);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int node_put(node_t *self, int index, node_base_t *node) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (node == NULL) ||
            ((index < 0) && (index > self->nodes))) {

            cause_error(E_INVPARM);

        }

        stat = self->_put(self, index, node);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int node_get_message(node_t *self, long msgnum, char **buffer) {

    int stat = OK;

    when_error_in {

        if (self == NULL) {

            cause_error(E_INVPARM);

        }

        stat = self->_get_message(self, msgnum, buffer);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int node_put_message(node_t *self, char *buffer, long *msgnum) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (buffer == NULL) || (msgnum == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_put_message(self, buffer, msgnum);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int node_extend(node_t *self, int amount) {

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

int node_find(node_t *self, void *data, int len, int (*compare)(void *, int, node_base_t *), int *index) {

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

int node_search(node_t *self, void *data, int len, int (*compare)(void *, int, node_base_t *), queue *results) {

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

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

int _node_ctor(object_t *object, item_list_t *items) {

    int stat = OK;
    int nodes = 2;
    char seq[256];
    char path[256];
    char msgseq[256];
    int timeout = 1;
    char nodedb[256];
    int retries = 30;
    node_t *self = NULL;
    tracer_t *dump = NULL;

    if (object != NULL) {

        memset(path, '\0', 256);
        memset(nodedb, '\0', 256);
        memset(msgseq, '\0', 256);

        /* capture our items */

        if (items != NULL) {

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case NODE_K_PATH: {
                        memcpy(&path, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case NODE_K_NODES: {
                        memcpy(&nodes, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case NODE_K_RETRIES: {
                        memcpy(&retries, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case NODE_K_TIMEOUT: {
                        memcpy(&timeout, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case NODE_K_TRACE: {
                        dump = items[x].buffer_address; 
                        break;
                    }
                }

            }

        }

        /* initilize our base klass here */

        object_set_error1(object, OK);

        /* initialize our derived klass here */

        self = NODE(object);

        /* assign our methods here */

        self->ctor = _node_ctor;
        self->dtor = _node_dtor;
        self->_compare = _node_compare;
        self->_override = _node_override;

        self->_get    = _node_get;
        self->_put    = _node_put;
        self->_open   = _node_open;
        self->_lock   = _node_lock;
        self->_next   = _node_next;
        self->_prev   = _node_prev;
        self->_last   = _node_last;
        self->_read   = _node_read;
        self->_write  = _node_write;
        self->_first  = _node_first;
        self->_build  = _node_build;
        self->_close  = _node_close;
        self->_unlock = _node_unlock;
        self->_extend = _node_extend;
        self->_normalize = _node_normalize;
        self->_get_message  = _node_get_message;
        self->_put_message  = _node_put_message;
        self->_get_sequence = _node_get_sequence;
        self->_find = _node_find;
        self->_search = _node_search;
        
        /* initialize internal variables here */

        when_error_in {

            self->index = 0;
            self->trace = dump;
            self->nodes = nodes;
            self->locked = FALSE;
            self->retries = retries;
            self->timeout = timeout;
            self->path = strdup(path);

            strncpy(nodedb, fnm_build(1, FnmPath, "nodes", ".dat", path, NULL), 255);
            self->nodedb = files_create(nodedb, retries, timeout);
            check_creation(self->nodedb);

            strncpy(seq, fnm_build(1, FnmPath, "nodes", ".seq", path, NULL), 255);
            self->sequence = files_create(seq, retries, timeout);
            check_creation(self->sequence);

            strncpy(msgseq, fnm_build(1, FnmPath, "node-msgs", ".seq", path, NULL), 255);
            self->msgseq = files_create(msgseq, retries, timeout);
            check_creation(self->msgseq);

            exit_when;

        } use {

            stat = ERR;
            process_error(self);

        } end_when;

    }

    return stat;

}

int _node_dtor(object_t *object) {

    int stat = OK;
    node_t *self = NODE(object);

    /* free local resources here */

    files_close(self->nodedb);
    files_close(self->sequence);
    files_close(self->msgseq);
    free(self->path);

    /* walk the chain, freeing as we go */

    object_demote(object, object_t);
    object_destroy(object);

    return stat;

}

int _node_override(node_t *self, item_list_t *items) {

    int stat = ERR;

    if (items != NULL) {

        int x;
        for (x = 0;; x++) {

            if ((items[x].buffer_length == 0) &&
                (items[x].item_code == 0)) break;

            switch(items[x].item_code) {
                case NODE_M_DESTRUCTOR: {
                    self->dtor = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case NODE_M_OPEN: {
                    self->_open = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case NODE_M_CLOSE: {
                    self->_close = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case NODE_M_UNLOCK: {
                    self->_unlock = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case NODE_M_LOCK: {
                    self->_lock = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case NODE_M_GET: {
                    self->_get = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case NODE_M_PUT: {
                    self->_put = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case NODE_M_NEXT: {
                    self->_next = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case NODE_M_PREV: {
                    self->_prev = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case NODE_M_LAST: {
                    self->_last = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case NODE_M_WRITE: {
                    self->_write = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case NODE_M_FIRST: {
                    self->_first = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case NODE_M_BUILD: {
                    self->_build = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case NODE_M_EXTEND: {
                    self->_extend = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case NODE_M_NORMALIZE: {
                    self->_normalize = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case NODE_M_FIND: {
                    self->_find = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case NODE_M_SEARCH: {
                    self->_search = items[x].buffer_address;
                    stat = OK;
                    break;
                }
            }

        }

    }

    return stat;

}

int _node_compare(node_t *self, node_t *other) {

    int stat = ERR;

    if ((object_compare(OBJECT(self), OBJECT(other)) == 0) &&
        (self->ctor == other->ctor) &&
        (self->dtor == other->dtor) &&
        (self->_compare == other->_compare) &&
        (self->_override == other->_override) &&
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
        (self->_unlock == other->_unlock) &&
        (self->_extend == other->_extend) &&
        (self->_normalize == other->_normalize) &&
        (self->nodes  == other->nodes) &&
        (self->locked == other->locked) &&
        (self->nodedb == other->nodedb) &&
        (self->sequence == other->sequence) &&
        (self->msgseq == other->msgseq) &&
        (self->trace  == other->trace)) {

        stat = OK;

    }

    return stat;

}

int _node_open(node_t *self) {

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

        stat = files_exists(self->msgseq, &exists);
        check_return(stat, self->msgseq);

        if (exists) {

            stat = files_open(self->msgseq, flags, 0);
            check_return(stat, self->msgseq);

        } else {

            stat = files_open(self->msgseq, create, mode);
            check_return(stat, self->msgseq);

            stat = files_write(self->msgseq, &sequence, sizeof(long), &count);
            check_return(stat, self->msgseq);

            if (count != sizeof(long)) {

                cause_error(EIO);

            }

        }

        stat = files_exists(self->nodedb, &exists);
        check_return(stat, self->nodedb);

        if (exists) {

            stat = files_open(self->nodedb, flags, 0);
            check_return(stat, self->nodedb);

        } else {

            stat = files_open(self->nodedb, create, mode);
            check_return(stat, self->nodedb);

            stat = self->_extend(self, self->nodes);
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

int _node_close(node_t *self) {

    int stat = OK;

    when_error_in {

        stat = files_close(self->nodedb);
        check_return(stat, self->nodedb);

        stat = files_close(self->sequence);
        check_return(stat, self->sequence);

        stat = files_close(self->msgseq);
        check_return(stat, self->msgseq);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _node_first(node_t *self, node_base_t *node, ssize_t *count) {

    int stat = OK;
    node_base_t ondisk;
    ssize_t position = 0;

    when_error_in {

        stat = files_seek(self->nodedb, 0, SEEK_SET);
        check_return(stat, self->nodedb);

        stat = files_tell(self->nodedb, &position);
        check_return(stat, self->nodedb);

        self->index = NODE_RECORD(position);

        stat = self->_lock(self, 0);
        check_return(stat, self);

        stat = self->_read(self, &ondisk, count);
        check_return(stat, self);

        stat = self->_unlock(self);
        check_return(stat, self);

        if (*count == sizeof(node_base_t)) {

            stat = self->_build(self, &ondisk, node);
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

int _node_next(node_t *self, node_base_t *node, ssize_t *count) {

    int stat = OK;
    node_base_t ondisk;
    ssize_t position = 0;

    when_error_in {

        stat = files_tell(self->nodedb, &position);
        check_return(stat, self);

        self->index = NODE_RECORD(position);

        stat = self->_lock(self, position);
        check_return(stat, self);

        stat = self->_read(self, &ondisk, count);
        check_return(stat, self);

        stat = self->_unlock(self);
        check_return(stat, self);

        if (*count == sizeof(node_base_t)) {

            stat = self->_build(self, &ondisk, node);
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

int _node_prev(node_t *self, node_base_t *node, ssize_t *count) {

    int stat = OK;
    node_base_t ondisk;
    ssize_t position = 0;
    off_t offset = sizeof(node_base_t);

    when_error_in {

        stat = files_tell(self->nodedb, &position);
        check_return(stat, self->nodedb);

        if (position > 0) {

            stat = files_seek(self->nodedb, -offset, SEEK_CUR);
            check_return(stat, self->nodedb);

            stat = self->_lock(self, position - offset);
            check_return(stat, self);

            self->index = NODE_RECORD(position - offset);

            stat = self->_read(self, &ondisk, count);
            check_return(stat, self);

            stat = self->_unlock(self);
            check_return(stat, self);

            stat = files_seek(self->nodedb, -offset, SEEK_CUR);
            check_return(stat, self->nodedb);

            if (*count == sizeof(node_base_t)) {

                stat = self->_build(self, &ondisk, node);
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

int _node_last(node_t *self, node_base_t *node, ssize_t *count) {

    int stat = OK;
    node_base_t ondisk;
    ssize_t position = 0;
    off_t offset = sizeof(node_base_t);

    when_error_in {

        stat = files_seek(self->nodedb, 0, SEEK_END);
        check_return(stat, self->nodedb);

        stat = files_seek(self->nodedb, -offset, SEEK_CUR);
        check_return(stat, self->nodedb);

        stat = files_tell(self->nodedb, &position);
        check_return(stat, self->nodedb);

        self->index = NODE_RECORD(position);

        stat = self->_lock(self, position);
        check_return(stat, self);

        stat = self->_read(self, &ondisk, count);
        check_return(stat, self);

        stat = self->_unlock(self);
        check_return(stat, self);

        stat = files_seek(self->nodedb, -offset, SEEK_CUR);
        check_return(stat, self->nodedb);

        if (*count == sizeof(node_base_t)) {

            stat = self->_build(self, &ondisk, node);
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

int _node_get(node_t *self, int index, node_base_t *node) {

    int stat = OK;
    ssize_t count = 0;
    node_base_t ondisk;
    off_t offset = NODE_OFFSET(index);

    when_error_in {

        stat = files_seek(self->nodedb, offset, SEEK_SET);
        check_return(stat, self->nodedb);

        self->_lock(self, offset);
        check_return(stat, self);

        stat = self->_read(self, &ondisk, &count);
        check_return(stat, self);

        stat = self->_unlock(self);
        check_return(stat, self);
        
        if (count == sizeof(node_base_t)) {

            stat = self->_build(self, &ondisk, node);
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

int _node_put(node_t *self, int index, node_base_t *node) {

    int stat = OK;
    ssize_t count = 0;
    node_base_t ondisk;
    off_t offset = NODE_OFFSET(index);
    off_t recsize = sizeof(node_base_t);

    when_error_in {

        stat = files_seek(self->nodedb, offset, SEEK_SET);
        check_return(stat, self->nodedb);

        stat = self->_lock(self, offset);
        check_return(stat, self);

        stat = self->_read(self, &ondisk, &count);
        check_return(stat, self);

        if (ondisk.revision > node->revision) {

            stat = self->_normalize(self, &ondisk, node);
            check_return(stat, self);

        } else {

            node->revision = ondisk.revision + 1;

        }

        stat = files_seek(self->nodedb, -recsize, SEEK_CUR);
        check_return(stat, self->nodedb);

        stat = self->_write(self, node, &count);
        check_return(stat, self);

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

int _node_get_message(node_t *self, long msgnum, char **buffer) {

    char name[32];
    int stat = OK;
    int exists = 0;
    ssize_t size = 0;
    ssize_t count = 0;
    char filename[256];
    int flags = O_RDWR;
    files_t *message = NULL;

    when_error_in {

        memset(name, '\0', 32);
        memset(filename, '\0', 256);
        snprintf(name, 31, "%ld", msgnum);

        strncpy(filename, fnm_build(1, FnmPath, name, ".msg", self->path, NULL), 255);
        message = files_create(filename, self->retries, self->timeout);
        check_creation(message);

        stat = files_exists(message, &exists);
        check_return(stat, message);

        if (! exists) {

            cause_error(EIO);

        }

        stat = files_size(message, &size);
        check_return(stat, message);

        errno = 0;
        *buffer = calloc(1, size + 1);
        if (*buffer == NULL) {

            cause_error(ENOMEM);

        }

        stat = files_open(message, flags, 0);
        check_return(stat, message);

        stat = files_read(message, *buffer, size, &count);
        check_return(stat, message);

        stat = files_close(message);
        check_return(stat, message);

        stat = files_unlink(message);
        check_return(stat, message);

        stat = files_destroy(message);
        check_return(stat, message);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _node_put_message(node_t *self, char *buffer, long *msgnum) {

    char name[32];
    int stat = OK;
    long junk = 0;
    ssize_t count = 0;
    char filename[256];
    files_t *message = NULL;
    int mode = (S_IRWXU | S_IRWXG);
    int create = (O_RDWR | O_CREAT);

    when_error_in {

        stat = self->_get_sequence(self, self->msgseq, &junk);
        check_return(stat, self);

        memset(name, '\0', 32);
        memset(filename, '\0', 256);
        snprintf(name, 31, "%ld", junk);

        strncpy(filename, fnm_build(1, FnmPath, name, ".msg", self->path, NULL), 255);
        message = files_create(filename, self->retries, self->timeout);
        check_creation(message);

        stat = files_open(message, create, mode);
        check_return(stat, message);

        stat = files_write(message, buffer, strlen(buffer), &count);
        check_return(stat, message);

        stat = files_close(message);
        check_return(stat, message);

        stat = files_destroy(message);
        check_return(stat, message);

        if (count != strlen(buffer)) {

            cause_error(EIO);

        }

        *msgnum = junk;

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _node_lock(node_t *self, off_t offset) {

    int stat = OK;
    int length = sizeof(node_base_t);

    when_error_in {

        stat = files_lock(self->nodedb, offset, length);
        check_return(stat, self->nodedb);

        self->locked = TRUE;

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _node_unlock(node_t *self) {

    int stat = OK;

    when_error_in {

        stat = files_unlock(self->nodedb);
        check_return(stat, self->nodedb);

        self->locked = FALSE;

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _node_get_sequence(node_t *self, files_t *file, long *sequence) {

    int stat = OK;
    long buffer = 0;
    ssize_t count = 0;
    int locked = FALSE;

    when_error_in {

        stat = files_seek(file, 0, SEEK_SET);
        check_return(stat, file);

        stat = files_lock(file, 0, sizeof(long));
        check_return(stat, file);

        locked = TRUE;

        stat = files_read(file, &buffer, sizeof(long), &count);
        check_return(stat, file);

        if (count != sizeof(long)) {

            cause_error(EIO);

        }

        *sequence = buffer;
        buffer++;

        stat = files_seek(file, 0, SEEK_SET);
        check_return(stat, file);

        stat = files_write(file, &buffer, sizeof(long), &count);
        check_return(stat, file);

        if (count != sizeof(long)) {

            cause_error(EIO);

        }

        stat = files_unlock(file);
        check_return(stat, file);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (locked) files_unlock(file);

    } end_when;

    return stat;

}

int _node_find(node_t *self, void *data, int len, int (*compare)(void *, int, node_base_t *), int *index) {

    int stat = OK;
    ssize_t count = 0;
    node_base_t ondisk;

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

int _node_search(node_t *self, void *data, int len, int (*compare)(void *, int, node_base_t *), queue *results) {

    int stat = OK;
    ssize_t count = 0;
    node_base_t ondisk;
    node_search_t *result = NULL;

    when_error_in {

        stat = self->_first(self, &ondisk, &count);
        check_return(stat, self);

        while (count > 0) {

            if (compare(data, len, &ondisk)) {

                errno = 0;
                result = calloc(1, sizeof(node_search_t));
                if (result == NULL) {

                    cause_error(errno);

                }

                result->nodenum = ondisk.nodenum;
                result->useron  = ondisk.useron;
                result->index   = self->index;

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

int _node_extend(node_t *self, int amount) {

    int stat = OK;
    node_base_t node;
    int revision = 1;
    long sequence = 0;
    ssize_t count = 0;
    ssize_t position = 0;

    when_error_in {

        memset(&node, '\0', sizeof(node_base_t));

        stat = files_seek(self->nodedb, 0, SEEK_END);
        check_return(stat, self->nodedb);

        int x;
        for (x = 0; x < amount; x++) {

            stat = self->_get_sequence(self, self->sequence, &sequence);
            check_return(stat, self);

            node.status = NODE_OFFLINE;
            node.nodenum = sequence;
            node.revision = revision;

            stat = files_tell(self->nodedb, &position);
            check_return(stat, self->nodedb);

            stat = self->_lock(self, position);
            check_return(stat, self);

            stat = self->_write(self, &node, &count);
            check_return(stat, self);

            stat = self->_unlock(self);
            check_return(stat, self);

            if (count != sizeof(node_base_t)) {

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

int _node_normalize(node_t *self, node_base_t *ondisk, node_base_t *node) {

    (*node).status = ondisk->status;
    (*node).errors = ondisk->errors;
    (*node).action = ondisk->action;
    (*node).pad1   = ondisk->pad1;  
    (*node).pad2   = ondisk->pad2; 
    (*node).useron = ondisk->useron;
    (*node).status = ondisk->status;
    (*node).misc   = ondisk->misc;
    (*node).aux    = ondisk->aux;
    (*node).extaux = ondisk->extaux;
    (*node).msgnum = ondisk->msgnum;
    (*node).nodenum = ondisk->nodenum;
    (*node).revision = ondisk->revision + 1;

    return OK;

}

int _node_build(node_t *self, node_base_t *ondisk, node_base_t *node) {

    (*node).status = ondisk->status;
    (*node).errors = ondisk->errors;
    (*node).action = ondisk->action;
    (*node).pad1   = ondisk->pad1;  
    (*node).pad2   = ondisk->pad2; 
    (*node).useron = ondisk->useron;
    (*node).status = ondisk->status;
    (*node).misc   = ondisk->misc;
    (*node).aux    = ondisk->aux;
    (*node).extaux = ondisk->extaux;
    (*node).msgnum = ondisk->msgnum;
    (*node).nodenum = ondisk->nodenum;
    (*node).revision = ondisk->revision;

    return OK;

}

int _node_read(node_t *self, node_base_t *node, ssize_t *count) {

    int stat = OK;

    when_error_in {

        stat = files_read(self->nodedb, node, sizeof(node_base_t), count);
        check_return(stat, self->nodedb);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _node_write(node_t *self, node_base_t *node, ssize_t *count) {

    int stat = OK;

    when_error_in {

        stat = files_write(self->nodedb, node, sizeof(node_base_t), count);
        check_return(stat, self->nodedb);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

