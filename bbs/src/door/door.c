
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

#include "include/when.h"
#include "include/item_list.h"

#include "rms/rms.h"
#include "files/files.h"
#include "tracer/tracer.h"

#include "bbs/src/bitops.h"
#include "bbs/src/door/door.h"

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _door_add(rms_t *self, door_base_t *door) {

    int stat = OK;
    ssize_t count = 0;
    door_base_t ondisk;
    int created = FALSE;

    when_error_in {

        stat = self->_first(self, &ondisk, &count);
        check_return(stat, self);

        while (count > 0) {

            if (bit_test(ondisk.flags, DF_DELETED)) {

                door->flags = 0;

                stat = self->_put(self, self->record, door);
                check_return(stat, self);

                created = TRUE;
                break;

            }

            stat = self->_next(self, &ondisk, &count);
            check_return(stat, self);

        }

        if (! created) {

            cause_error(EOVERFLOW);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}
 
int _door_build(rms_t *self, door_base_t *ondisk, door_base_t *door) {

    strncpy((*door).name, ondisk->name, DOOR_NAME_LEN);
    strncpy((*door).description, ondisk->description, DOOR_DESC_LEN);
    strncpy((*door).command, ondisk->command, DOOR_CMD_LEN);
    (*door).flags = ondisk->flags;
    (*door).revision = ondisk->revision;

    return OK;

}
 
int _door_del(rms_t *self, off_t recnum) {

    int stat = OK;
    ssize_t count = 0;
    door_base_t ondisk;
    off_t recsize = self->recsize;
    off_t offset = RMS_OFFSET(recnum, self->recsize);

    when_error_in {

        stat = files_seek(self->rmsdb, offset, SEEK_SET);
        check_return(stat, self->rmsdb);

        stat = self->_lock(self, offset);
        check_return(stat, self);

        stat = self->_read(self, &ondisk, &count);
        check_return(stat, self);

        if (count != self->recsize) {

            cause_error(EIO);

        }

        ondisk.flags |= DF_DELETED;
        ondisk.revision++;

        stat = files_seek(self->rmsdb, -recsize, SEEK_CUR);
        check_return(stat, self->rmsdb);

        stat = self->_write(self, &ondisk, &count);
        check_return(stat, self);

        if (count != self->recsize) {

            cause_error(EIO);

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
 
int _door_extend(rms_t *self, int amount) {

    int stat = OK;
    door_base_t door;
    int revision = 1;
    ssize_t count = 0;
    ssize_t position = 0;

    when_error_in {

        memset(&door, '\0', sizeof(door_base_t));

        /* defaults */

        door.revision = revision;
        door.flags |= DF_DELETED;

        stat = files_seek(self->rmsdb, 0, SEEK_END);
        check_return(stat, self->rmsdb);

        int x = 0;
        for (; x < amount; x++) {

            stat = files_tell(self->rmsdb, &position);
            check_return(stat, self->rmsdb);

            stat = self->_lock(self, position);
            check_return(stat, self);

            stat = self->_write(self, &door, &count);
            check_return(stat, self);

            stat = self->_unlock(self);
            check_return(stat, self);

            if (count != self->recsize) {

                cause_error(EIO);

            }

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (self->locked) self->_unlock(self);

    } end_when;

    return stat;

}

int _door_normalize(rms_t *self, door_base_t *ondisk, door_base_t *door) {

    strncpy((*door).name, ondisk->name, DOOR_NAME_LEN);
    strncpy((*door).description, ondisk->description, DOOR_DESC_LEN);
    strncpy((*door).command, ondisk->command, DOOR_CMD_LEN);
    (*door).flags = ondisk->flags;
    (*door).revision = ondisk->revision + 1;

    return OK;

}

int _door_put(rms_t *self, off_t recnum, door_base_t *door) {

    int stat = OK;
    ssize_t count = 0;
    door_base_t ondisk;
    off_t recsize = self->recsize;
    off_t offset = RMS_OFFSET(recnum, self->recsize);

    when_error_in {

        stat = files_seek(self->rmsdb, offset, SEEK_SET);
        check_return(stat, self->rmsdb);

        stat = self->_lock(self, offset);
        check_return(stat, self);

        stat = self->_read(self, &ondisk, &count);
        check_return(stat, self);

        if (ondisk.revision > door->revision) {

            stat = self->_normalize(self, &ondisk, door);
            check_return(stat, self);

        } else {

            door->revision = ondisk.revision + 1;

        }

        stat = files_seek(self->rmsdb, -recsize, SEEK_CUR);
        check_return(stat, self->rmsdb);

        stat = self->_write(self, door, &count);
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

int door_capture(rms_t *self, void *data, queue *results) {

    int stat = OK;
    door_base_t *ondisk = NULL;
    door_search_t *result = NULL;

    when_error_in {

        ondisk = (door_base_t *)data;

        errno = 0;
        result = calloc(1, sizeof(door_search_t));
        if (result == NULL) cause_error(errno);

        strncpy(result->name, ondisk->name, DOOR_NAME_LEN);
        result->record = self->record;

        stat = que_push_head(results, result);
        check_status(stat, QUE_OK, E_NOQUEUE);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;
    
    return stat;

}

char *door_version(rms_t *self) {

    char *version = VERSION;

    return version;

}

rms_t *door_create(char *path, int records, int retries, int timeout, tracer_t *dump) {

    int stat = OK;
    rms_t *self = NULL;
    char *name = "doors";
    item_list_t items[8];
    int recsize = sizeof(door_base_t);

    when_error_in {

        self = rms_create(path, name, records, recsize, retries, timeout, dump);
        check_creation(self);

        SET_ITEM(items[0], RMS_M_ADD, _door_add, 0, NULL);
        SET_ITEM(items[1], RMS_M_BUILD, _door_build, 0, NULL);
        SET_ITEM(items[2], RMS_M_DEL, _door_del, 0, NULL);
        SET_ITEM(items[3], RMS_M_EXTEND, _door_extend, 0, NULL);
        SET_ITEM(items[4], RMS_M_NORMALIZE, _door_normalize, 0, NULL);
        SET_ITEM(items[5], RMS_M_PUT, _door_put, 0, NULL);
        SET_ITEM(items[6], 0, 0, 0, 0);

        stat = rms_override(self, items);
        check_return(stat, self);

        exit_when;

    } use {

        process_error(self);

    } end_when;

    return self;

}

