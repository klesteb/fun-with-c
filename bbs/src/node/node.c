
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

#include "rms/rms.h"
#include "include/when.h"
#include "objects/object.h"
#include "include/error_codes.h"

#include "bbs/src/node/node.h"

require_klass(RMS_KLASS);

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _node_build(rms_t *self, node_base_t *ondisk, node_base_t *node) {

    (*node).action = ondisk->action;
    (*node).useron = ondisk->useron;
    (*node).nodenum = ondisk->nodenum;
    (*node).revision = ondisk->revision;

    return OK;

}

int _node_extend(rms_t *self, int amount) {

    int stat = OK;
    node_base_t node;
    int revision = 1;
    long sequence = 0;
    ssize_t count = 0;
    ssize_t position = 0;

    when_error_in {

        memset(&node, '\0', sizeof(node_base_t));

        stat = self->_seek(self, 0, SEEK_END);
        check_return(stat, self);

        int x;
        for (x = 0; x < amount; x++) {

            stat = self->_get_sequence(self, &sequence);
            check_return(stat, self);

            node.useron = 0;
            node.action = NODE_OFFL;
            node.nodenum = sequence;
            node.revision = revision;

            stat = self->_tell(self, &position);
            check_return(stat, self);

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

int _node_normalize(rms_t *self, node_base_t *ondisk, node_base_t *node) {

    (*node).action = ondisk->action;
    (*node).useron = ondisk->useron;
    (*node).nodenum = ondisk->nodenum;
    (*node).revision = ondisk->revision + 1;

    return OK;

}

int _node_put(rms_t *self, off_t record, node_base_t *node) {

    int stat = OK;
    ssize_t count = 0;
    node_base_t ondisk;
    off_t recsize = self->recsize;
    off_t offset = RMS_OFFSET(record, self->recsize);

    when_error_in {

        stat = self->_seek(self, offset, SEEK_SET);
        check_return(stat, self);

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

        stat = self->_seek(self, -recsize, SEEK_CUR);
        check_return(stat, self);

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

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

int node_capture(rms_t *self, void *data, queue *results) {

    int stat = OK;
    node_base_t *ondisk = NULL;
    node_search_t *result = NULL;

    when_error_in {

        ondisk = (node_base_t *)data;

        errno = 0;
        result = calloc(1, sizeof(node_search_t));
        if (result == NULL) cause_error(errno);

        result->nodenum = ondisk->nodenum;
        result->action  = ondisk->action;
        result->useron  = ondisk->useron;
        result->record  = self->record;

        stat = que_push_head(results, result);
        check_status(stat, QUE_OK, E_NOQUEUE);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

char *node_version(rms_t *self) {

    char *version = VERSION;

    return version;

}

rms_t *node_create(char *path, int records, int retries, int timeout, tracer_t *dump) {

    int stat = ERR;
    rms_t *self = NULL;
    char *name = "nodes";
    item_list_t items[5];
    int recsize = sizeof(node_base_t);

    when_error_in {

        self = rms_create(path, name, records, recsize, retries, timeout, dump);
        check_creation(self);

        SET_ITEM(items[0], RMS_M_BUILD, _node_build, 0, NULL);
        SET_ITEM(items[1], RMS_M_EXTEND, _node_extend, 0, NULL);
        SET_ITEM(items[2], RMS_M_NORMALIZE, _node_normalize, 0, NULL);
        SET_ITEM(items[3], RMS_M_PUT, _node_put, 0, NULL);
        SET_ITEM(items[4], 0, 0, 0, 0);

        stat = rms_override(self, items);
        check_return(stat, self);

        exit_when;

    } use {

        process_error(self);

    } end_when;

    return self;

}

