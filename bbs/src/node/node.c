
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

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

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

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

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

        object_set_error2(us, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

char *node_version(node_t *self) {

    char *version = VERSION;

    return version;

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

        /* initialize internal variables here */

        when_error_in {

            strncpy(nodedb, fnm_build(1, FnmPath, "node", ".dab", path, NULL), 255);
            self->nodedb = files_create(nodedb, retries, timeout);
            check_creation(self->nodedb);

            self->trace = dump;
            self->nodes = nodes;

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
        (self->_override == other->_override)) {

        stat = OK;

    }

    return stat;

}

