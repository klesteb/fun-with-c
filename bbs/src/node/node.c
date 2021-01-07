
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
int _node_get(node_t *, int, node_base_t *);
int _node_put(node_t *, int, node_base_t *);
int _node_next(node_t *, node_base_t *, ssize_t *);
int _node_prev(node_t *, node_base_t *, ssize_t *);
int _node_last(node_t *, node_base_t *, ssize_t *);
int _node_read(node_t *, node_base_t *, ssize_t *);
int _node_write(node_t *, node_base_t *, ssize_t *);
int _node_first(node_t *, node_base_t *, ssize_t *);
int _node_build(node_t *, node_base_t *, node_base_t *);

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

int node_first(node_t *self, node_base_t *node, ssize_t *count) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (node == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_first(self, node, count);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int node_next(node_t *self, node_base_t *node, ssize_t *count) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (node == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_next(self, node, count);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int node_prev(node_t *self, node_base_t *node, ssize_t *count) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (node == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_prev(self, node, count);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int node_last(node_t *self, node_base_t *node, ssize_t *count) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (node == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_last(self, node, count);
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
            ((index >= 0) && (index <= self->nodes))) {

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
            ((index >= 0) && (index <= self->nodes))) {

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

int node_index(node_t *self, int *index) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (index == NULL)) {

            cause_error(E_INVPARM);

        }

        *index = self->index;

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
    char path[256];
    int timeout = 1;
    char nodedb[256];
    int retries = 30;
    node_t *self = NULL;
    tracer_t *dump = NULL;

    if (object != NULL) {

        memset(path, '\0', 256);
        memset(nodedb, '\0', 256);

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

        /* initialize internal variables here */

        when_error_in {

            self->index = 0;
            self->trace = dump;
            self->nodes = nodes;
            self->locked = FALSE;

            strncpy(nodedb, fnm_build(1, FnmPath, "node", ".dab", path, NULL), 255);
            self->nodedb = files_create(nodedb, retries, timeout);
            check_creation(self->nodedb);

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
        (self->nodes  == other->nodes) &&
        (self->locked == other->locked) &&
        (self->nodedb == other->nodedb) &&
        (self->trace  == other->trace)) {

        stat = OK;

    }

    return stat;

}

int _node_open(node_t *self) {

    int stat = OK;
    int exists = 0;
    node_base_t node;
    ssize_t count = 0;
    int flags = O_RDWR;
    int create = (O_RDWR | O_CREAT);
    int mode = (S_IRWXU | S_IRWXG);

    when_error_in {

        stat = files_exists(self->nodedb, &exists);
        check_return(stat, self->nodedb);

        if (exists) {

            stat = files_open(self->nodedb, flags, 0);
            check_return(stat, self->nodedb);

        } else {

            stat = files_open(self->nodedb, create, mode);
            check_return(stat, self->nodedb);

            stat = self->_lock(self, 0);
            check_return(stat, self);

            memset(&node, '\0', sizeof(node_base_t));
            node.status = NODE_OFFLINE;

            int x;
            for (x = 0; x < self->nodes; x++) {

                stat = self->_write(self, &node, &count);
                check_return(stat, self);

                if (count != sizeof(node_base_t)) {

                    cause_error(EIO);

                }

            }

            stat = self->_unlock(self);
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

    when_error_in {

        stat = files_seek(self->nodedb, offset, SEEK_SET);
        check_return(stat, self->nodedb);

        stat = self->_lock(self, offset);
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

int _node_build(node_t *self, node_base_t *ondisk, node_base_t *node) {

    int stat = OK;

    (*node).status = ondisk->status;
    (*node).errors = ondisk->errors;
    (*node).action = ondisk->action;
    (*node).pad1   = ondisk->pad1;  
    (*node).useron = ondisk->useron;
    (*node).connection = ondisk->connection; 
    (*node).status = ondisk->status;
    (*node).misc   = ondisk->misc;
    (*node).aux    = ondisk->aux;
    (*node).extaux = ondisk->extaux;

    return stat;

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

