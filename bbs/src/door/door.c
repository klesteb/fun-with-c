
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

#include "rms/rms.h"
#include "include/when.h"
#include "tracer/tracer.h"
#include "include/item_list.h"

#include "bbs/src/bitops.h"
#include "bbs/src/door/door.h"

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _door_build(rms_t *self, door_base_t *ondisk, door_base_t *record) {

    memset(record, '\0', self->recsize);

    strncpy(record->command, ondisk->command, DOOR_CMD_LEN);
    strncpy(record->clean, ondisk->clean, DOOR_CMD_LEN);
    strncpy(record->path, ondisk->path, DOOR_PATH_LEN);

    record->type = ondisk->type;
    record->flags = ondisk->flags;
    record->cost = ondisk->cost;
    record->active = ondisk->active;
    record->revision = ondisk->revision;

    int x = 0;
    for (; x < USERNUM; x++) {

        record->usage[x].runs = ondisk->usage[x].runs;
        record->usage[x].wasted = ondisk->usage[x].wasted;

    }

    return OK;

}

int _door_init(rms_t *self) {
    
    int stat = OK;
    ssize_t count = 0;
    door_base_t ondisk;
    ssize_t position = 0;
    off_t recsize = self->recsize;

    when_error_in {

        /* defaults */

        memset(&ondisk, '\0', recsize);

        ondisk.type = 0;
        ondisk.cost = 0;
        ondisk.flags = 0;
        ondisk.revision = 1;
        ondisk.active = FALSE;

        int x = 0;
        for (; x < USERNUM; x++) {

            ondisk.usage[x].runs = 0;
            ondisk.usage[x].wasted = 0;

        }

        /* start at the beginning */

        stat = self->_seek(self, 0, SEEK_SET);
        check_return(stat, self);

        stat = self->_tell(self, &position);
        check_return(stat, self);

        stat = self->_lock(self, position);
        check_return(stat, self);

        stat = self->_write(self, &ondisk, &count);
        check_return(stat, self);

        stat = self->_unlock(self);
        check_return(stat, self);

        if (count != recsize) {

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

int _door_normalize(rms_t *self, door_base_t *ondisk, door_base_t *record) {

    int x = 0;
    for (; x < USERNUM; x++) {

        if (ondisk->usage[x].runs > record->usage[x].runs) {

            record->usage[x].runs = ondisk->usage[x].runs;

        }

        if (ondisk->usage[x].wasted > record->usage[x].wasted) {

            record->usage[x].wasted = ondisk->usage[x].wasted;

        }

    }

    record->revision = ondisk->revision + 1;

    return OK;

}

int _door_put(rms_t *self, off_t recnum, door_base_t *record) {

    int stat = OK;
    ssize_t count = 0;
    door_base_t ondisk;
    off_t recsize = self->recsize;
    off_t offset = RMS_OFFSET(recnum, self->recsize);

    when_error_in {

        stat = self->_seek(self, offset, SEEK_SET);
        check_return(stat, self);

        stat = self->_lock(self, offset);
        check_return(stat, self);

        stat = self->_read(self, &ondisk, &count);
        check_return(stat, self);

        if (count != recsize) {

            cause_error(EIO);

        }

        if (ondisk.revision > record->revision) {

            stat = self->_normalize(self, &ondisk, record);
            check_return(stat, self);

        } else {

            record->revision = ondisk.revision + 1;

        }

        stat = self->_seek(self, -recsize, SEEK_CUR);
        check_return(stat, self);

        stat = self->_write(self, record, &count);
        check_return(stat, self);

        stat = self->_unlock(self);
        check_return(stat, self);

        if (count != recsize) {

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

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

char *door_version(rms_t *self) {

    char *version = VERSION;

    return version;

}

rms_t *door_create(char *path, char *name, int retries, int timeout, tracer_t *dump) {

    int stat = OK;
    int records = 1;
    rms_t *self = NULL;
    item_list_t items[5];
    int recsize = sizeof(door_base_t);

    when_error_in {

        self = rms_create(path, name, records, recsize, retries, timeout, dump);
        check_creation(self);

        SET_ITEM(items[0], RMS_M_PUT, _door_put, 0, NULL);
        SET_ITEM(items[1], RMS_M_INIT, _door_init, 0, NULL);
        SET_ITEM(items[2], RMS_M_BUILD, _door_build, 0, NULL);
        SET_ITEM(items[3], RMS_M_NORMALIZE, _door_normalize, 0, NULL);
        SET_ITEM(items[4], 0, 0, 0, 0);

        stat = rms_override(self, items);
        check_return(stat, self);

        exit_when;

    } use {

        process_error(self);

    } end_when;

    return self;

}

