
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

#include "rms.h"
#include "when.h"
#include "files.h"
#include "status.h"
#include "tracer.h"
#include "item_list.h"

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _status_add(rms_t *self, room_status_t *status) {

    int stat = OK;
    ssize_t count = 0;
    int created = FALSE;
    room_status_t ondisk;

    when_error_in {

        stat = self->_first(self, &ondisk, &count);
        check_return(stat, self);

        while (count > 0) {

            if ((ondisk.roomnum == 0) && (ondisk.usernum == 0)) {

                stat = self->_put(self, self->record, status);
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

        if (self->locked) self->_unlock(self);

    } end_when;

    return stat;

}
 
int _status_build(rms_t *self, room_status_t *ondisk, room_status_t *status) {

    (*status).roomnum = ondisk->roomnum;
    (*status).usernum = ondisk->usernum;
    (*status).flags = ondisk->flags;
    (*status).revision = ondisk->revision;

    return OK;

}
 
int _status_del(rms_t *self, off_t recnum) {

    int stat = OK;
    ssize_t count = 0;
    room_status_t ondisk;
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

        ondisk.roomnum = 0;
        ondisk.usernum = 0;
        ondisk.flags = 0;
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
 
int _status_extend(rms_t *self, int amount) {

    int stat = OK;
    ssize_t count = 0;
    ssize_t position = 0;
    room_status_t ondisk;

    when_error_in {

        memset(&ondisk, '\0', sizeof(room_status_t));

        /* defaults */

        ondisk.roomnum = 0;
        ondisk.usernum = 0;
        ondisk.flags = 0;
        ondisk.revision = 1;
        
        stat = files_seek(self->rmsdb, 0, SEEK_END);
        check_return(stat, self->rmsdb);

        int x;
        for (x = 0; x < amount; x++) {

            stat = files_tell(self->rmsdb, &position);
            check_return(stat, self->rmsdb);

            stat = self->_lock(self, position);
            check_return(stat, self);

            stat = self->_write(self, &ondisk, &count);
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

int _status_normalize(rms_t *self, room_status_t *ondisk, room_status_t *status) {

    (*status).roomnum = ondisk->roomnum;
    (*status).usernum = ondisk->usernum;
    (*status).flags = ondisk->flags;
    (*status).revision = ondisk->revision + 1;

    return OK;

}

int _status_put(rms_t *self, off_t recnum, room_status_t *status) {

    int stat = OK;
    ssize_t count = 0;
    room_status_t ondisk;
    off_t recsize = self->recsize;
    off_t offset = RMS_OFFSET(recnum, self->recsize);

    when_error_in {

        stat = files_seek(self->rmsdb, offset, SEEK_SET);
        check_return(stat, self->rmsdb);

        stat = self->_lock(self, offset);
        check_return(stat, self);

        stat = self->_read(self, &ondisk, &count);
        check_return(stat, self);

        if (ondisk.revision > status->revision) {

            stat = self->_normalize(self, &ondisk, status);
            check_return(stat, self);

        } else {

            status->revision = ondisk.revision + 1;

        }

        stat = files_seek(self->rmsdb, -recsize, SEEK_CUR);
        check_return(stat, self->rmsdb);

        stat = self->_write(self, status, &count);
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

int status_capture(rms_t *self, void *data, queue *results) {

    int stat = OK;
    room_status_t *ondisk = NULL;
    status_search_t *result = NULL;

    when_error_in {

        ondisk = (room_status_t *)data;

        errno = 0;
        result = calloc(1, sizeof(status_search_t));
        if (result == NULL) cause_error(errno);

        result->roomnum = ondisk->roomnum;
        result->usernum = ondisk->usernum;
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

char *status_version(rms_t *self) {

    char *version = VERSION;

    return version;

}

rms_t *status_create(char *path, int records, int retries, int timeout, tracer_t *dump) {

    int stat = OK;
    rms_t *self = NULL;
    item_list_t items[7];
    char *name = "room-status";
    int recsize = sizeof(room_status_t);

    when_error_in {

        self = rms_create(path, name, records, recsize, retries, timeout, dump);
        check_creation(self);

        SET_ITEM(items[0], RMS_M_ADD, _status_add, 0, NULL);
        SET_ITEM(items[1], RMS_M_BUILD, _status_build, 0, NULL);
        SET_ITEM(items[2], RMS_M_DEL, _status_del, 0, NULL);
        SET_ITEM(items[3], RMS_M_EXTEND, _status_extend, 0, NULL);
        SET_ITEM(items[4], RMS_M_NORMALIZE, _status_normalize, 0, NULL);
        SET_ITEM(items[5], RMS_M_PUT, _status_put, 0, NULL);
        SET_ITEM(items[6], 0, 0, 0, 0);

        stat = rms_override(self, items);
        check_return(stat, self);

        exit_when;

    } use {

        process_error(self);

    } end_when;

    return self;

}

