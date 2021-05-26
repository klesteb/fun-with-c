
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
#include <time.h>
#include <errno.h>

#include "jam.h"
#include "when.h"
#include "files.h"
#include "tracer.h"
#include "object.h"
#include "fnm_util.h"
#include "que_util.h"
#include "misc/misc.h"
#include "error_codes.h"

require_klass(OBJECT_KLASS);

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _jam_ctor(object_t *, item_list_t *);
int _jam_dtor(object_t *);
int _jam_compare(jam_t *, jam_t *);
int _jam_override(jam_t *, item_list_t *);

int _jam_lock(jam_t *);
int _jam_open(jam_t *);
int _jam_close(jam_t *);
int _jam_remove(jam_t *);
int _jam_unlock(jam_t *);
int _jam_size(jam_t *, ssize_t *);
int _jam_crc32(jam_t *, uchar *, ulong, ulong *);

int _jam_read_header(jam_t *, jam_header_t *);
int _jam_write_header(jam_t *, jam_header_t *);

int _jam_del_lastread(jam_t *, ulong);
int _jam_add_lastread(jam_t *, jam_lastread_t *);
int _jam_get_lastread(jam_t *, ulong, jam_lastread_t *);
int _jam_put_lastread(jam_t *, ulong, jam_lastread_t *);
int _jam_find_lastread(jam_t *, ulong, jam_lastread_t *);
int _jam_read_lastread(jam_t *, jam_lastread_t *, ssize_t *);
int _jam_write_lastread(jam_t *, jam_lastread_t *, ssize_t *);
int _jam_append_lastread(jam_t *, jam_header_t *, jam_lastread_t *);
int _jam_normalize_lastread(jam_t *, jam_lastread_t *, jam_lastread_t *);
int _jam_update_lastread(jam_t *, ulong, jam_header_t *, jam_lastread_t *);

int _jam_find_index(jam_t *, ulong, jam_index_t *);
int _jam_delete_index(jam_t *, ulong, jam_header_t *);
int _jam_read_index(jam_t *, jam_index_t *, ssize_t *);
int _jam_write_index(jam_t *, jam_index_t *, ssize_t *);
int _jam_append_index(jam_t *, jam_header_t *, jam_index_t *);
int _jam_normalize_index(jam_t *, jam_index_t *, jam_index_t *);
int _jam_update_index(jam_t *, ulong, jam_header_t *, jam_index_t *);
int _jam_search_index(jam_t *, ulong, int (*compare)(ulong, jam_index_t *), queue *);

int _jam_read_text(jam_t *, char **, int);
int _jam_write_text(jam_t *, char *, int);
int _jam_find_text(jam_t *, off_t, char **, int);
int _jam_append_text(jam_t *, char *, int, ssize_t *);

int _jam_del_message(jam_t *, ulong);
int _jam_new_message(jam_t *, jam_message_t **);
int _jam_read_message(jam_t *, jam_message_t *, queue *);
int _jam_write_message(jam_t *, jam_message_t *, queue *);
int _jam_put_message(jam_t *, ulong, jam_message_t *, queue *);
int _jam_find_message(jam_t *, off_t, jam_message_t *, queue *);
int _jam_add_message(jam_t *, jam_message_t *, queue *, char *);
int _jam_append_message(jam_t *, jam_message_t *, queue *, ssize_t *);
int _jam_get_message(jam_t *, ulong, jam_message_t *, queue *, char **);
int _jam_normalize_message(jam_t *, jam_message_t *, queue *, jam_message_t *, queue *);
int _jam_search_messages(jam_t *, ulong, int (*compare)(ulong, jam_message_t *), queue *);

int _jam_free_fields(jam_t *, queue *);
int _jam_free_field(jam_t *, jam_field_t *);
int _jam_read_fields(jam_t *, ulong, queue *);
int _jam_write_fields(jam_t *, queue *, int *);
int _jam_new_field(jam_t *, ushort, char *, jam_field_t **);

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

declare_klass(JAM_KLASS) {
    .size = KLASS_SIZE(jam_t),
    .name = KLASS_NAME(jam_t),
    .ctor = _jam_ctor,
    .dtor = _jam_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

jam_t *jam_create(char *path, char *name, int retries, int timeout, int base, tracer_t *dump) {

    int stat = ERR;
    jam_t *self = NULL;
    item_list_t items[7];

    SET_ITEM(items[0], JAM_K_PATH, path, strlen(path), NULL);
    SET_ITEM(items[1], JAM_K_NAME, name, strlen(name), NULL);
    SET_ITEM(items[2], JAM_K_RETRIES, &retries, sizeof(int), NULL);
    SET_ITEM(items[3], JAM_K_TIMEOUT, &timeout, sizeof(int), NULL);
    SET_ITEM(items[4], JAM_K_BASE, &base, sizeof(int), NULL);
    SET_ITEM(items[5], JAM_K_TRACE, dump, 0, NULL);
    SET_ITEM(items[6], 0, 0, 0, 0);

    self = (jam_t *)object_create(JAM_KLASS, items, &stat);

    return self;

}

int jam_destroy(jam_t *self) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            if (object_assert(self, jam_t)) {

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

int jam_override(jam_t *self, item_list_t *items) {

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

int jam_compare(jam_t *us, jam_t *them) {

    int stat = OK;

    when_error {

        if (us != NULL) {

            if (object_assert(them, jam_t)) {

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

char *jam_version(jam_t *self) {

    char *version = VERSION;

    return version;

}

int jam_open(jam_t *self) {

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

int jam_close(jam_t *self) {

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

int jam_remove(jam_t *self) {

    int stat = OK;

    when_error_in {

        if (self == NULL) {

            cause_error(E_INVPARM);

        }

        stat = self->_remove(self);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int jam_size(jam_t *self, ssize_t *size) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (size == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_lock(self);
        check_return(stat, self);

        stat = self->_size(self, size);
        check_return(stat, self);

        stat = self->_unlock(self);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int jam_crc32(jam_t *self, uchar *buffer, ulong length, ulong *result) {
    
    int stat = OK;

    when_error_in {

        if ((self == NULL) || (buffer == NULL) || (result == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_crc32(self, buffer, length, result);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int jam_del_lastread(jam_t *self, ulong user_crc) {

    int stat = OK;

    when_error_in {

        if (self == NULL) {

            cause_error(E_INVPARM);

        }

        stat = self->_del_lastread(self, user_crc);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int jam_add_lastread(jam_t *self, ulong user_id, ulong user_crc) {

    int stat = OK;
    jam_lastread_t lastread;

    when_error_in {

        if (self == NULL) {

            cause_error(E_INVPARM);

        }

        /* create a lastread record for the user */

        lastread.user_crc = user_crc;
        lastread.user_id = user_id;
        lastread.last_read_msg = 0;
        lastread.high_read_msg = 0;

        stat = self->_add_lastread(self, &lastread);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int jam_get_lastread(jam_t *self, ulong user_crc, jam_lastread_t *lastread) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (lastread == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_get_lastread(self, user_crc, lastread);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int jam_put_lastread(jam_t *self, ulong user_crc, jam_lastread_t *lastread) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (lastread == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_put_lastread(self, user_crc, lastread);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int jam_new_field(jam_t *self, ushort item, char *buffer, jam_field_t **field) {

    int stat = OK;

    when_error_in {

        if (self == NULL) {

            cause_error(E_INVPARM);

        }

        stat = self->_new_field(self, item, buffer, field);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int jam_free_field(jam_t *self, jam_field_t *field) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (field == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_free_field(self, field);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int jam_new_message(jam_t *self, jam_message_t **message) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (message == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_new_message(self, message);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int jam_get_message(jam_t *self, ulong msgnum, jam_message_t *message, queue *fields, char **text) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (message == NULL) || (fields == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_get_message(self, msgnum, message, fields, text);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int jam_add_message(jam_t *self, jam_message_t *message, queue *fields, char *text) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (message == NULL) || 
            (fields == NULL) || (text == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_add_message(self, message, fields, text);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int jam_del_message(jam_t *self, ulong msgnum) {

    int stat = OK;

    when_error_in {

        if (self == NULL) {

            cause_error(E_INVPARM);

        }

        stat = self->_del_message(self, msgnum);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int jam_put_message(jam_t *self, ulong msgnum, jam_message_t *message, queue *fields) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (message == NULL) || (fields == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_put_message(self, msgnum, message, fields);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int jam_search_messages(jam_t *self, ulong select, int (*compare)(ulong, jam_message_t *), queue *results) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (compare == NULL) || (results == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_search_messages(self, select, compare, results);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int jam_search_index(jam_t *self, ulong select, int (*compare)(ulong, jam_index_t *), queue *results) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (compare == NULL) || (results == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_search_index(self, select, compare, results);
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

int _jam_ctor(object_t *object, item_list_t *items) {

    int base = 1;
    int stat = OK;
    char path[1024];
    char name[1024];
    char header[1024];
    char text[1024];
    char index[1024];
    char lastread[1024];
    int retries = 10;
    int timeout = 30;
    jam_t *self = NULL;
    tracer_t *dump = NULL;

    if (object != NULL) {

        memset(name, '\0', 1024);
        memset(path, '\0', 1024);
        memset(header, '\0', 1024);
        memset(text, '\0', 1024);
        memset(index, '\0', 1024);
        memset(lastread, '\0', 1024);

        /* capture our items */

        if (items != NULL) {

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case JAM_K_PATH: {
                        memcpy(&path, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case JAM_K_NAME: {
                        memcpy(&name, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case JAM_K_RETRIES: {
                        memcpy(&retries, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case JAM_K_TIMEOUT: {
                        memcpy(&timeout, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case JAM_K_TRACE: {
                        dump = items[x].buffer_address; 
                        break;
                    }
                    case JAM_K_BASE: {
                        memcpy(&base, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        if (base < 1) base = 1;
                        break;
                    }
                }

            }

        }

        /* initilize our base klass here */

        object_set_error1(object, OK);

        /* initialize our derived klass here */

        self = JAM(object);

        /* assign our methods here */

        self->ctor      = _jam_ctor;
        self->dtor      = _jam_dtor;
        self->_compare  = _jam_compare;
        self->_override = _jam_override;

        self->_open   = _jam_open;
        self->_close  = _jam_close;
        self->_remove = _jam_remove;
        self->_size   = _jam_size;
        self->_lock   = _jam_lock;
        self->_unlock = _jam_unlock;
        self->_crc32  = _jam_crc32;

        self->_read_header  = _jam_read_header;
        self->_write_header = _jam_write_header;

        self->_append_index    = _jam_append_index;
        self->_delete_index    = _jam_delete_index;
        self->_find_index      = _jam_find_index;
        self->_normalize_index = _jam_normalize_index;
        self->_read_index      = _jam_read_index;
        self->_update_index    = _jam_update_index;
        self->_write_index     = _jam_write_index;
        self->_search_index    = _jam_search_index;

        self->_del_lastread       = _jam_del_lastread;
        self->_add_lastread       = _jam_add_lastread;
        self->_put_lastread       = _jam_put_lastread;
        self->_get_lastread       = _jam_get_lastread;
        self->_append_lastread    = _jam_append_lastread;
        self->_find_lastread      = _jam_find_lastread;
        self->_update_lastread    = _jam_update_lastread;
        self->_read_lastread      = _jam_read_lastread;
        self->_write_lastread     = _jam_write_lastread;
        self->_normalize_lastread = _jam_normalize_lastread;

        self->_find_text         = _jam_find_text;
        self->_append_text       = _jam_append_text;
        self->_read_text         = _jam_read_text;
        self->_write_text        = _jam_write_text;
        self->_add_message       = _jam_add_message;
        self->_del_message       = _jam_del_message;
        self->_get_message       = _jam_get_message;
        self->_put_message       = _jam_put_message;
        self->_find_message      = _jam_find_message;
        self->_append_message    = _jam_append_message;
        self->_write_message     = _jam_write_message;
        self->_read_message      = _jam_read_message;
        self->_new_message       = _jam_new_message;
        self->_read_fields       = _jam_read_fields;
        self->_write_fields      = _jam_write_fields;
        self->_free_fields       = _jam_free_fields;
        self->_new_field         = _jam_new_field;
        self->_free_field        = _jam_free_field;
        self->_search_messages   = _jam_search_messages;
        self->_normalize_message = _jam_normalize_message;

        /* initialize internal variables here */

        when_error_in {

            self->locked = 0;
            self->base = base;
            self->trace = dump;

            strncpy(header, fnm_build(1, FnmPath, name, EXT_HDRFILE, path, NULL), 255);
            self->header = files_create(header, retries, timeout);
            check_creation(self->header);

            strncpy(text, fnm_build(1, FnmPath, name, EXT_TXTFILE, path, NULL), 255);
            self->text = files_create(text, retries, timeout);
            check_creation(self->text);

            strncpy(index, fnm_build(1, FnmPath, name, EXT_IDXFILE, path, NULL), 255);
            self->index = files_create(index, retries, timeout);
            check_creation(self->index);

            strncpy(lastread, fnm_build(1, FnmPath, name, EXT_LRDFILE, path, NULL), 255);
            self->lastread = files_create(lastread, retries, timeout);
            check_creation(self->lastread);

            exit_when;

        } use {

            stat = ERR;
            process_error(self);

        } end_when;

    }

    return stat;

}

int _jam_dtor(object_t *object) {

    int stat = OK;
    jam_t *self = NULL;

    /* free local resources here */

    self = JAM(object);
    
    files_destroy(self->header);
    files_destroy(self->index);
    files_destroy(self->text);
    files_destroy(self->lastread);
    
    /* walk the chain, freeing as we go */

    object_demote(object, object_t);
    object_destroy(object);

    return stat;

}

int _jam_override(jam_t *self, item_list_t *items) {

    int stat = ERR;

    if (items != NULL) {

        int x;
        for (x = 0;; x++) {

            if ((items[x].buffer_length == 0) &&
                (items[x].item_code == 0)) break;

            switch(items[x].item_code) {
                case JAM_M_DESTRUCTOR: {
                    self->dtor = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_OPEN: {
                    self->_open = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_CLOSE: {
                    self->_close = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_REMOVE: {
                    self->_remove = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_SIZE: {
                    self->_size = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_LOCK: {
                    self->_lock = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_UNLOCK: {
                    self->_unlock = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_READ_HEADER: {
                    self->_read_header = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_WRITE_HEADER: {
                    self->_write_header = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_ADD_LASTREAD: {
                    self->_add_lastread = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_GET_LASTREAD: {
                    self->_get_lastread = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_PUT_LASTREAD: {
                    self->_put_lastread = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_READ_LASTREAD: {
                    self->_read_lastread = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_WRITE_LASTREAD: {
                    self->_write_lastread = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_NORMALIZE_LASTREAD: {
                    self->_normalize_lastread = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_READ_INDEX: {
                    self->_read_index = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_WRITE_INDEX: {
                    self->_write_index = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_FIND_INDEX: {
                    self->_find_index = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_APPEND_INDEX: {
                    self->_append_index = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_DELETE_INDEX: {
                    self->_delete_index = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_NORMALIZE_INDEX: {
                    self->_normalize_index = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_FIND_TEXT: {
                    self->_find_text = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_APPEND_TEXT: {
                    self->_append_text = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_READ_TEXT: {
                    self->_read_text = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_WRITE_TEXT: {
                    self->_write_text = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_ADD_MESSAGE: {
                    self->_add_message = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_GET_MESSAGE: {
                    self->_get_message = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_PUT_MESSAGE: {
                    self->_put_message = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_FIND_MESSAGE: {
                    self->_find_message = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_APPEND_MESSAGE: {
                    self->_append_message = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_READ_MESSAGE: {
                    self->_read_message = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_WRITE_MESSAGE: {
                    self->_write_message = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_READ_FIELDS: {
                    self->_read_fields = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_WRITE_FIELDS: {
                    self->_write_fields = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_FREE_FIELDS: {
                    self->_free_fields = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_NEW_FIELD: {
                    self->_new_field = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_NEW_MESSAGE: {
                    self->_new_message = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_DEL_LASTREAD: {
                    self->_del_lastread = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_SEARCH_MESSAGES: {
                    self->_search_messages = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_NORMALIZE_MESSAGE: {
                    self->_normalize_message = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case JAM_M_SEARCH_INDEX: {
                    self->_search_index = items[x].buffer_address;
                    stat = OK;
                    break;
                }
            }

        }

    }

    return stat;

}

int _jam_compare(jam_t *self, jam_t *other) {

    int stat = ERR;

    if ((object_compare(OBJECT(self), OBJECT(other)) == 0) &&
        (self->ctor                == other->ctor) &&
        (self->dtor                == other->dtor) &&
        (self->_compare            == other->_compare) &&
        (self->_override           == other->_override) &&
        (self->_open               == other->_open) &&
        (self->_close              == other->_close) &&
        (self->_remove             == other->_remove) &&
        (self->_size               == other->_size) &&
        (self->_lock               == other->_lock) &&
        (self->_unlock             == other->_unlock) &&
        (self->_crc32              == other->_crc32) &&
        (self->_read_header        == other->_read_header) &&
        (self->_write_header       == other->_write_header) &&
        (self->_append_index       == other->_append_index) &&
        (self->_delete_index       == other->_delete_index) &&
        (self->_find_index         == other->_find_index) &&
        (self->_normalize_index    == other->_normalize_index) &&
        (self->_read_index         == other->_read_index) &&
        (self->_update_index       == other->_update_index) &&
        (self->_write_index        == other->_write_index) &&
        (self->_search_index       == other->_search_index) &&
        (self->_del_lastread       == other->_del_lastread) &&
        (self->_add_lastread       == other->_add_lastread) &&
        (self->_put_lastread       == other->_put_lastread) &&
        (self->_get_lastread       == other->_get_lastread) &&
        (self->_append_lastread    == other->_append_lastread) &&
        (self->_find_lastread      == other->_find_lastread) &&
        (self->_update_lastread    == other->_update_lastread) &&
        (self->_read_lastread      == other->_read_lastread) &&
        (self->_write_lastread     == other->_write_lastread) &&
        (self->_normalize_lastread == other->_normalize_lastread) &&
        (self->_find_text          == other->_find_text) &&
        (self->_append_text        == other->_append_text) &&
        (self->_read_text          == other->_read_text) &&
        (self->_write_text         == other->_write_text) &&
        (self->_add_message        == other->_add_message) &&
        (self->_del_message        == other->_del_message) &&
        (self->_get_message        == other->_get_message) &&
        (self->_put_message        == other->_put_message) &&
        (self->_find_message       == other->_find_message) &&
        (self->_append_message     == other->_append_message) &&
        (self->_write_message      == other->_write_message) &&
        (self->_read_message       == other->_read_message) &&
        (self->_new_message        == other->_new_message) &&
        (self->_read_fields        == other->_read_fields) &&
        (self->_write_fields       == other->_write_fields) &&
        (self->_free_fields        == other->_free_fields) &&
        (self->_new_field          == other->_new_field) &&
        (self->_free_field         == other->_free_field) &&
        (self->_search_messages    == other->_search_messages) &&
        (self->_normalize_message  == other->_normalize_message) &&
        (self->locked              == other->locked) &&
        (self->trace               == other->trace) &&
        (self->base                == other->base)) {

        stat = OK;

    }

    return stat;

}

int _jam_open(jam_t *self) {

    int stat = OK;
    int exists = 0;
    int flags = O_RDWR;
    jam_header_t base;
    int create = (O_RDWR | O_CREAT);
    int mode = (S_IRWXU | S_IRWXG);

    when_error_in {

        stat = files_exists(self->header, &exists);
        check_return(stat, self->header);

        if (exists) {

            stat = files_open(self->header, flags, 0);
            check_return(stat, self->header);

        } else {

            stat = files_open(self->header, create, mode);
            check_return(stat, self->header);

            base.date_created = time(NULL);
            base.mod_counter = 0;
            base.active_msgs = 0;
            base.password_crc = JAM_NO_CRC;
            base.base_msg_num = self->base;
            memset(&base.rsrvd, '\0', sizeof(base.rsrvd));

            stat = self->_write_header(self, &base);
            check_return(stat, self);

        }

        stat = files_exists(self->index, &exists);
        check_return(stat, self->index);

        if (exists) {

            stat = files_open(self->index, flags, 0);
            check_return(stat, self->index);

        } else {

            stat = files_open(self->index, create, mode);
            check_return(stat, self->index);

        }

        stat = files_exists(self->text, &exists);
        check_return(stat, self->text);

        if (exists) {

            stat = files_open(self->text, flags, 0);
            check_return(stat, self->text);

        } else {

            stat = files_open(self->text, create, mode);
            check_return(stat, self->text);

        }

        stat = files_exists(self->lastread, &exists);
        check_return(stat, self->lastread);

        if (exists) {

            stat = files_open(self->lastread, flags, 0);
            check_return(stat, self->lastread);

        } else {

            stat = files_open(self->lastread, create, mode);
            check_return(stat, self->lastread);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _jam_close(jam_t *self) {

    int stat = OK;

    when_error_in {

        stat = files_close(self->header);
        check_return(stat, self->header);

        stat = files_close(self->index);
        check_return(stat, self->index);

        stat = files_close(self->text);
        check_return(stat, self->text);

        stat = files_close(self->lastread);
        check_return(stat, self->lastread);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _jam_remove(jam_t *self) {

    int stat = OK;

    when_error_in {

        stat = files_close(self->header);
        check_return(stat, self->header);

        stat = files_unlink(self->header);
        check_return(stat, self->header);

        stat = files_close(self->index);
        check_return(stat, self->index);

        stat = files_unlink(self->index);
        check_return(stat, self->index);

        stat = files_close(self->text);
        check_return(stat, self->text);

        stat = files_unlink(self->text);
        check_return(stat, self->text);

        stat = files_close(self->lastread);
        check_return(stat, self->lastread);

        stat = files_unlink(self->lastread);
        check_return(stat, self->lastread);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _jam_size(jam_t *self, ssize_t *size) {

    int stat = OK;
    off_t offset = 0;

    when_error_in {

        *size = 0;

        stat = files_seek(self->index, 0, SEEK_END);
        check_return(stat, self->index);

        stat = files_tell(self->index, &offset);
        check_return(stat, self->index);

        if (offset >= sizeof(jam_index_t)) {

            *size = offset / sizeof(jam_index_t);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _jam_lock(jam_t *self) {

    int stat = OK;

    when_error_in {

        stat = files_lock(self->header, 0, 1);
        check_return(stat, self->header);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _jam_unlock(jam_t *self) {

    int stat = OK;

    when_error_in {

        stat = files_unlock(self->header);
        check_return(stat, self->header);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

/*----------------------------------------------------------------*/
/* header routines                                                */
/*----------------------------------------------------------------*/

int _jam_write_header(jam_t *self, jam_header_t *header) {

    int stat = OK;
    ssize_t count = 0;

    when_error_in {

        stat = files_seek(self->header, 0, SEEK_SET);
        check_return(stat, self->header);

        stat = files_write(self->header, header, sizeof(jam_header_t), &count);
        check_return(stat, self->header);

        if (count != sizeof(jam_header_t)) {

            cause_error(EIO);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _jam_read_header(jam_t *self, jam_header_t *header) {

    int stat = OK;
    ssize_t count = 0;

    when_error_in {

        stat = files_seek(self->header, 0, SEEK_SET);
        check_return(stat, self->header);

        stat = files_read(self->header, header, sizeof(jam_header_t), &count);
        check_return(stat, self->header);

        if (count != sizeof(jam_header_t)) {

            cause_error(EIO);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);
        
    } end_when;

    return stat;

}

/*----------------------------------------------------------------*/
/* lastread routines                                              */
/*----------------------------------------------------------------*/

int _jam_add_lastread(jam_t *self, jam_lastread_t *lastread) { 

    int stat = OK;
    jam_header_t header;
    jam_lastread_t junk;

    when_error_in {

        /* lock the message base */

        stat = self->_lock(self);
        check_return(stat, self);

        /* retrieve the header */

        stat = self->_read_header(self, &header);
        check_return(stat, self);

        /* append the new lastread record */

        stat = self->_find_lastread(self, lastread->user_crc, &junk);
        check_return(stat, self);

        if (junk.user_crc == JAM_NO_CRC) {

            stat = self->_append_lastread(self, &header, lastread);
            check_return(stat, self);

        }

        /* update the header */

        stat = self->_write_header(self, &header);
        check_return(stat, self);

        /* unlock the message base */

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

int _jam_del_lastread(jam_t *self, ulong user_crc) { 

    int stat = OK;
    ssize_t count = 0;
    ssize_t offset = 0;
    jam_header_t header;
    jam_lastread_t junk;

    when_error_in {

        /* lock the message base */

        stat = self->_lock(self);
        check_return(stat, self);

        /* retrieve the header */

        stat = self->_read_header(self, &header);
        check_return(stat, self);

        /* delete the new lastread record */

        stat = self->_find_lastread(self, user_crc, &junk);
        check_return(stat, self);

        if (junk.user_crc == user_crc) {

            stat = files_tell(self->lastread, &offset);
            check_return(stat, self->lastread);

            offset -= sizeof(jam_lastread_t);

            stat = files_seek(self->lastread, offset, SEEK_SET);
            check_return(stat, self->lastread);

            junk.user_id = JAM_NO_CRC;
            junk.user_crc = JAM_NO_CRC;
            junk.last_read_msg = 0;
            junk.high_read_msg = 0;

            stat = self->_write_lastread(self, &junk, &count);
            check_return(stat, self);

            if (count != sizeof(jam_lastread_t)) {
                
                cause_error(EIO);
                
            }

            header.mod_counter++;
            self->revision = header.mod_counter;

        }

        /* update the header */

        stat = self->_write_header(self, &header);
        check_return(stat, self);

        /* unlock the message base */

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

int _jam_get_lastread(jam_t *self, ulong user_crc, jam_lastread_t *lastread) {

    int stat = OK;

    when_error_in {

        /* lock the message base */

        stat = self->_lock(self);
        check_return(stat, self);

        /* find the record */

        stat = self->_find_lastread(self, user_crc, lastread);
        check_return(stat, self);
        
        /* unlock the message base */

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

int _jam_put_lastread(jam_t *self, ulong user_crc, jam_lastread_t *lastread) {

    int stat = OK;
    jam_header_t header;

    when_error_in {

        /* lock the message base */

        stat = self->_lock(self);
        check_return(stat, self);

        /* retrieve the header */

        stat = self->_read_header(self, &header);
        check_return(stat, self);

        /* update the record */

        stat = self->_update_lastread(self, user_crc, &header, lastread);
        check_return(stat, self);

        /* update the header */

        stat = self->_write_header(self, &header);
        check_return(stat, self);

        /* unlock the message base */

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

int _jam_find_lastread(jam_t *self, ulong user_crc, jam_lastread_t *lastread) {

    int stat = OK;
    ssize_t count = 0;
    jam_lastread_t junk;

    when_error_in {

        /* set defaults */

        (*lastread).user_id = 0;
        (*lastread).last_read_msg = 0;
        (*lastread).high_read_msg = 0;
        (*lastread).user_crc = JAM_NO_CRC;

        /* find the record */

        stat = files_seek(self->lastread, 0, SEEK_SET);
        check_return(stat, self->lastread);

        do {

            stat = self->_read_lastread(self, &junk, &count);
            check_return(stat, self);

            if (user_crc == junk.user_crc) {

                (*lastread).user_id       = junk.user_id;
                (*lastread).user_crc      = junk.user_crc;
                (*lastread).last_read_msg = junk.last_read_msg;
                (*lastread).high_read_msg = junk.high_read_msg;

                break;

            }

        } while (count > 0);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _jam_update_lastread(jam_t *self, ulong user_crc, jam_header_t *header, jam_lastread_t *lastread) {

    int stat = OK;
    ssize_t count = 0;
    ssize_t offset = 0;
    jam_lastread_t junk;
    int updated = FALSE;

    when_error_in {

        stat = files_seek(self->lastread, 0, SEEK_SET);
        check_return(stat, self->lastread);

        do {

            stat = self->_read_lastread(self, &junk, &count);
            check_return(stat, self);

            if (user_crc == junk.user_crc) {

                if (header->mod_counter > self->revision) {

                    stat = self->_normalize_lastread(self, &junk, lastread);
                    check_return(stat, self);

                }

                offset = sizeof(jam_lastread_t);
                stat = files_seek(self->lastread, -offset, SEEK_CUR);
                check_return(stat, self->lastread);

                stat = self->_write_lastread(self, lastread, &count);
                check_return(stat, self);

                updated = TRUE;
                (*header).mod_counter++;
                self->revision = header->mod_counter;

                break;

            }

        } while (count > 0);

        if (! updated) {

            stat = self->_append_lastread(self, header, lastread);
            check_return(stat, self);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _jam_append_lastread(jam_t *self, jam_header_t *header, jam_lastread_t *lastread) {

    int stat = OK;
    ssize_t count = 0;

    when_error_in {

        stat = files_seek(self->lastread, 0, SEEK_END);
        check_return(stat, self->lastread);

        stat = self->_write_lastread(self, lastread, &count);
        check_return(stat, self);

        if (count != sizeof(jam_lastread_t)) {

            cause_error(EIO);

        }

        (*header).mod_counter++;
        self->revision = header->mod_counter;

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _jam_write_lastread(jam_t *self, jam_lastread_t *lastread, ssize_t *count) {

    int stat = OK;

    when_error_in {

        stat = files_write(self->lastread, lastread, sizeof(jam_lastread_t), count);
        check_return(stat, self->lastread);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _jam_read_lastread(jam_t *self, jam_lastread_t *lastread, ssize_t *count) {

    int stat = OK;

    when_error_in {

        stat = files_read(self->lastread, lastread, sizeof(jam_lastread_t), count);
        check_return(stat, self->lastread);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _jam_normalize_lastread(jam_t *self, jam_lastread_t *temp, jam_lastread_t *lastread) {

    int stat = OK;

    if (temp->last_read_msg > lastread->last_read_msg) {

        (*lastread).last_read_msg = temp->last_read_msg;

    }

    if (temp->high_read_msg > lastread->high_read_msg) {

        (*lastread).high_read_msg = temp->high_read_msg;

    }

    return stat;

}

/*----------------------------------------------------------------*/
/* index routines                                                 */
/*----------------------------------------------------------------*/

int _jam_append_index(jam_t *self, jam_header_t *header, jam_index_t *index) {

    int stat = OK;
    ssize_t count = 0;

    when_error_in {

        stat = files_seek(self->index, 0, SEEK_END);
        check_return(stat, self->index);

        stat = self->_write_index(self, index, &count);
        check_return(stat, self);

        if (count != sizeof(jam_index_t)) {

            cause_error(EIO);

        }

        (*header).mod_counter++;
        self->revision = header->mod_counter;

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _jam_delete_index(jam_t *self, ulong msgnum, jam_header_t *header) {

    int stat = OK;
    jam_index_t junk;
    ssize_t count = 0;
    off_t offset = msgnum * sizeof(jam_index_t);

    when_error_in {

        junk.user_crc      = JAM_NO_CRC;
        junk.header_offset = JAM_NO_CRC;

        /* find the record */

        stat = files_seek(self->index, offset, SEEK_SET);
        check_return(stat, self->index);

        stat = self->_write_index(self, &junk, &count);
        check_return(stat, self);

        if (count != sizeof(jam_index_t)) {

            cause_error(EIO);

        }

        (*header).mod_counter++;
        self->revision = header->mod_counter;

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _jam_find_index(jam_t *self, ulong msgnum, jam_index_t *index) {

    int stat = OK;
    jam_index_t junk;
    ssize_t count = 0;
    off_t offset = msgnum * sizeof(jam_index_t);

    when_error_in {

        /* find the record */

        stat = files_seek(self->index, offset, SEEK_SET);
        check_return(stat, self->index);

        stat = self->_read_index(self, &junk, &count);
        check_return(stat, self);

        (*index).user_crc      = junk.user_crc;
        (*index).header_offset = junk.header_offset;

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _jam_update_index(jam_t *self, ulong msgnum, jam_header_t *header, jam_index_t *index) {

    int stat = OK;
    ssize_t count = 0;
    jam_index_t temp;
    off_t offset = msgnum * sizeof(jam_index_t);

    when_error_in {

        stat = files_seek(self->index, offset, SEEK_SET);
        check_return(stat, self->index);

        /* check for possible revisions */

        if (header->mod_counter > self->revision) {

            /* retrieve the old index */

            stat = self->_read_index(self, &temp, &count);
            check_return(stat, self);

            if (count != sizeof(jam_index_t)) {

                cause_error(EIO);

            }

            /* merge changes */

            stat = self->_normalize_index(self, &temp, index);
            check_return(stat, self);

            /* reposition the file pointer */

            offset = sizeof(jam_index_t);
            stat = files_seek(self->index, -offset, SEEK_CUR);
            check_return(stat, self->index);

        }

        /* update the index */

        stat = self->_write_index(self, index, &count);
        check_return(stat, self);

        if (count != sizeof(jam_index_t)) {

            cause_error(EIO);

        }

        (*header).mod_counter++;
        self->revision = header->mod_counter;

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}
                    
int _jam_read_index(jam_t *self, jam_index_t *index, ssize_t *count) {

    int stat = OK;

    when_error_in {

        stat = files_read(self->index, index, sizeof(jam_index_t), count);
        check_return(stat, self->index);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _jam_write_index(jam_t *self, jam_index_t *index, ssize_t *count) {

    int stat = OK;

    when_error_in {

        stat = files_write(self->index, index, sizeof(jam_index_t), count);
        check_return(stat, self->index);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _jam_normalize_index(jam_t *self, jam_index_t *temp, jam_index_t *index) {

    int stat = OK;

    if (temp->header_offset != index->header_offset) {

        (*index).header_offset = temp->header_offset;
                
    }

    return stat;

}

int _jam_search_index(jam_t *self, ulong selector, int (*compare)(ulong, jam_index_t *), queue *results) {

    int stat = OK;
    ssize_t msgs = 0;
    jam_index_t index;
    jam_search_t *search = NULL;

    when_error_in {

        /* lock the message base */

        stat = self->_lock(self);
        check_return(stat, self);

        /* get the number of messages in the msgbase */

        stat = self->_size(self, &msgs);
        check_return(stat, self);

        /* walk the index */

        int x;
        for (x = 0; x < msgs; x++) {

            stat = self->_find_index(self, x, &index);
            check_return(stat, self);

            /* skip deleted messages */

            if ((index.user_crc == JAM_NO_CRC) &&
                (index.header_offset == JAM_NO_CRC)) {

                continue;

            }

            if (compare(selector, &index)) {

                errno = 0;
                search = calloc(1, sizeof(jam_search_t));
                if (search == NULL) {

                    cause_error(errno);

                }

                search->msgnum = x;
                search->offset = index.header_offset;

                stat = que_push_tail(results, search);
                check_status(stat, QUE_OK, E_NOQUEUE);

            }

        }

        /* unlock the message base */

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
/* message routines                                               */
/*----------------------------------------------------------------*/

int _jam_del_message(jam_t *self, ulong msgnum) {

    int stat = OK;
    queue oldfields;
    ulong length = 0;
    char *text = NULL;
    jam_index_t index;
    ssize_t offset = 0;
    jam_header_t header;
    jam_message_t oldmsg;

    when_error_in {

        stat = que_init(&oldfields);
        check_status(stat, QUE_OK, E_INVOPS);

        /* lock the message base */

        stat = self->_lock(self);
        check_return(stat, self);

        /* retrieve the header */

        stat = self->_read_header(self, &header);
        check_return(stat, self);

        /* retrieve the index */

        stat = self->_find_index(self, msgnum, &index);
        check_return(stat, self);

        /* retrieve the message */

        stat = self->_find_message(self, index.header_offset, &oldmsg, &oldfields);
        check_return(stat, self);

        /* mark it deleted and write it back out */

        oldmsg.text_length = 0;
        oldmsg.attribute |= MSG_DELETED;

        stat = files_seek(self->header, index.header_offset, SEEK_SET);
        check_return(stat, self->header);

        stat = self->_write_message(self, &oldmsg, &oldfields);
        check_return(stat, self);

        stat = self->_free_fields(self, &oldfields);
        check_return(stat, self);

        /* erase the message text */

        offset = oldmsg.text_offset;
        length = oldmsg.text_length;

        errno = 0;
        if ((text = calloc(1, length)) == NULL) {

            cause_error(errno);

        }

        stat = files_seek(self->text, offset, SEEK_SET);
        check_return(stat, self);

        stat = self->_write_text(self, text, length);
        check_return(stat, self);

        free(text);

        /* mark the index entry as "deleted" */
        
        stat = self->_delete_index(self, msgnum, &header);
        check_return(stat, self);

        /* update the header */

        header.active_msgs--;

        stat = self->_write_header(self, &header);
        check_return(stat, self);

        /* unlock the message base */

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

int _jam_add_message(jam_t *self, jam_message_t *message, queue *fields, char *text) {

    int stat = OK;
    jam_index_t index;
    ssize_t offset = 0;
    ssize_t records = 0;
    jam_header_t header;
    jam_field_t *field = NULL;

    when_error_in {

        index.user_crc = JAM_NO_CRC;
        index.header_offset = JAM_NO_CRC;

        /* lock the message base */

        stat = self->_lock(self);
        check_return(stat, self);

        /* retrieve the header */

        stat = self->_read_header(self, &header);
        check_return(stat, self);

        /* append the text records */

        stat = self->_append_text(self, text, strlen(text), &offset);
        check_return(stat, self);

        stat = self->_size(self, &records);
        check_return(stat, self);

        /* calculate message values */

        message->reserved_word = 0;
        message->text_offset = offset;
        message->revision = CURRENTREBLEV;
        message->text_length = strlen(text);
        memcpy(&message->signature, HEADERSIGNATURE, 4);
        message->msgnum = records + header.base_msg_num;

        /* get the crcs for various fields */

        for (field = que_first(fields);
             field != NULL;
             field = que_next(fields)) {

            message->subfield_length += sizeof(jam_save_field_t) + field->data_length;

            if (field->lo_id == JAMSFLD_MSGID) {

                trim(field->buffer);
                self->_crc32(self, field->buffer, strlen(field->buffer), &message->msg_id_crc);

            }

            if (field->lo_id == JAMSFLD_REPLYID) {

                trim(field->buffer);
                self->_crc32(self, field->buffer, strlen(field->buffer), &message->reply_crc);

            }

            if (field->lo_id == JAMSFLD_SENDERNAME) {

                trim(field->buffer);
                self->_crc32(self, field->buffer, strlen(field->buffer), &index.user_crc);

            }

        }

        /* write message out */

        stat = self->_append_message(self, message, fields, &offset);
        check_return(stat, self);

        /* create an index for this record */

        index.header_offset = offset;

        stat = self->_append_index(self, &header, &index);
        check_return(stat, self);

        /* update the header */

        header.active_msgs++;

        stat = self->_write_header(self, &header);
        check_return(stat, self);

        /* unlock the message base */

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

int _jam_get_message(jam_t *self, ulong msgnum, jam_message_t *message, queue *fields, char **text) {

    int stat = OK;
    int length = 0;
    ssize_t offset = 0;
    jam_index_t index;
    jam_header_t header;

    when_error_in {

        /* lock the message base */

        stat = self->_lock(self);
        check_return(stat, self);

        /* retrieve the header */

        stat = self->_read_header(self, &header);
        check_return(stat, self);

        /* retrieve the index for this msgnum */

        stat = self->_find_index(self, msgnum, &index);
        check_return(stat, self);

        /* retrieve the message header */

        stat = self->_find_message(self, index.header_offset, message, fields);
        check_return(stat, self);

        /* retrieve the message text */

        offset = message->text_offset;
        length = message->text_length;

        errno = 0;
        *text = calloc(1, length + 1);
        if (*text == NULL) {

            cause_error(errno);

        }

        stat = self->_find_text(self, offset, text, length);
        check_return(stat, self);

        /* update the header */

        stat = self->_write_header(self, &header);
        check_return(stat, self);

        /* unlock the message base */

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

int _jam_put_message(jam_t *self, ulong msgnum, jam_message_t *message, queue *fields) {

    int stat = OK;
    queue oldfields;
    jam_index_t index;
    ssize_t offset = 0;
    jam_header_t header;
    jam_message_t oldmsg;

    when_error_in {

        /* lock the message base */

        stat = self->_lock(self);
        check_return(stat, self);

        /* retrieve the header */

        stat = self->_read_header(self, &header);
        check_return(stat, self);

        /* retrieve the index for this msgnum */

        stat = self->_find_index(self, msgnum, &index);
        check_return(stat, self);

        /* retrieve the old message header */

        stat = que_init(&oldfields);
        check_status(stat, QUE_OK, E_INVOPS);

        stat = self->_find_message(self, index.header_offset, &oldmsg, &oldfields);
        check_return(stat, self);

        if (message->subfield_length > oldmsg.subfield_length) {

            if (header.mod_counter > self->revision) {

                stat = self->_normalize_message(self, &oldmsg, &oldfields, message, fields);
                check_return(stat, self);

            }

            /* mark it deleted and write it back out */

            oldmsg.text_length = 0;
            oldmsg.attribute |= MSG_DELETED;

            stat = files_seek(self->header, index.header_offset, SEEK_SET);
            check_return(stat, self->header);

            stat = self->_write_message(self, &oldmsg, &oldfields);
            check_return(stat, self);

            stat = self->_free_fields(self, &oldfields);
            check_return(stat, self);

            /* append the updated message */

            memcpy(&message->signature, HEADERSIGNATURE, 4);

            stat = self->_append_message(self, message, fields, &offset);
            check_return(stat, self);

            /* update the index */

            index.header_offset = offset;

            stat = self->_update_index(self, msgnum, &header, &index);
            check_return(stat, self);

        } else {

            if (header.mod_counter > self->revision) {

                stat = self->_normalize_message(self, &oldmsg, &oldfields, message, fields);
                check_return(stat, self);

            }

            stat = files_seek(self->header, index.header_offset, SEEK_SET);
            check_return(stat, self->header);

            stat = self->_write_message(self, message, fields);
            check_return(stat, self);

        }

        /* update the header */

        stat = self->_write_header(self, &header);
        check_return(stat, self);

        /* unlock the message base */

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

int _jam_search_messages(jam_t *self, ulong selector, int (*compare)(ulong, jam_message_t *), queue *results) {

    queue fields;
    int stat = OK;
    ssize_t msgs = 0;
    jam_index_t index;
    jam_message_t message;
    jam_search_t *search = NULL;

    when_error_in {

        /* lock the message base */

        stat = self->_lock(self);
        check_return(stat, self);

        /* get the number of messages in the msgbase */

        stat = self->_size(self, &msgs);
        check_return(stat, self);

        /* walk the index */

        int x;
        for (x = 0; x < msgs; x++) {

            stat = self->_find_index(self, x, &index);
            check_return(stat, self);

            /* skip deleted messages */

            if ((index.user_crc == JAM_NO_CRC) &&
                (index.header_offset == JAM_NO_CRC)) {

                continue;

            }

            stat = que_init(&fields);
            check_status(stat, QUE_OK, E_INVOPS);

            stat = self->_find_message(self, index.header_offset, &message, &fields);
            check_return(stat, self);

            if (compare(selector, &message)) {

                errno = 0;
                search = calloc(1, sizeof(jam_search_t));
                if (search == NULL) {

                    cause_error(errno);

                }

                search->msgnum = x;
                search->offset = index.header_offset;

                stat = que_push_tail(results, search);
                check_status(stat, QUE_OK, E_NOQUEUE);

            }

            stat = self->_free_fields(self, &fields);
            check_return(stat, self);

        }

        /* unlock the message base */

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

int _jam_new_message(jam_t *self, jam_message_t **message) {

    int stat = OK;

    when_error_in {

        errno = 0;
        *message = calloc(1, sizeof(jam_message_t));
        if (*message == NULL) {

            cause_error(errno);

        }

        (**message).times_read = 0;
        (**message).msg_id_crc = 0;
        (**message).reply_crc = 0;
        (**message).reply_to = 0;
        (**message).reply_1st = 0;
        (**message).reply_next = 0;
        (**message).date_written = 0;
        (**message).date_received = 0;
        (**message).date_processed = 0;
        (**message).msgnum = 0;
        (**message).attribute = 0;
        (**message).attribute2 = 0;
        (**message).text_offset = 0;
        (**message).text_length = 0;
        (**message).password_crc = 0;
        (**message).cost = 0;

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _jam_read_fields(jam_t *self, ulong size, queue *fields) {

    int pos = 0;
    int stat = OK;
    ssize_t count = 0;
    uchar *data = NULL;
    uchar *buffer = NULL;
    jam_save_field_t junk;
    int jsst = sizeof(jam_save_field_t);

    when_error_in {

        errno = 0;
        if ((buffer = calloc(1, size)) == NULL) {

            cause_error(errno);

        }

        stat = files_read(self->header, buffer, size, &count);
        check_return(stat, self->header);

        if (count == size) {

            while (pos < size) {

                errno = 0;
                if ((memcpy(&junk, &buffer[pos], jsst)) == NULL) {

                    cause_error(errno);

                }

                pos += jsst;

                errno = 0;
                jam_field_t *field = calloc(1, sizeof(jam_field_t));
                if (field == NULL) {

                    cause_error(errno);

                }

                field->lo_id       = junk.lo_id;
                field->hi_id       = junk.hi_id;
                field->data_length = junk.data_length;

                if (field->data_length > 0) {

                    errno = 0;
                    if ((data = calloc(1, field->data_length)) == NULL) {

                        cause_error(errno);

                    }

                    errno = 0;
                    if ((memcpy(data, &buffer[pos], field->data_length)) == NULL) {

                        cause_error(errno);

                    }

                    field->buffer = data;
                    pos += field->data_length;

                    stat = que_push_tail(fields, field);
                    check_status(stat, QUE_OK, E_NOQUEUE);

                }

            }

        } else {

            cause_error(EIO);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    if (buffer != NULL) free(buffer);

    return stat;
    
}

int _jam_write_fields(jam_t *self, queue *fields, int *size) {

    int stat = OK;
    ssize_t count = 0;
    jam_save_field_t junk;
    jam_field_t *temp = NULL;
    int jsst = sizeof(jam_save_field_t);

    when_error_in {

        *size = 0;

        for (temp = que_first(fields);
             temp != NULL;
             temp = que_next(fields)) {

            junk.lo_id       = temp->lo_id;
            junk.hi_id       = temp->hi_id;
            junk.data_length = temp->data_length;

            stat = files_write(self->header, &junk, jsst, &count);
            check_return(stat, self->header);

            if (count != jsst) {

                cause_error(EIO);

            }

            if (temp->data_length > 0) {

                stat = files_write(self->header, temp->buffer, temp->data_length, &count);
                check_return(stat, self->header);

                if (count != temp->data_length) {

                    cause_error(EIO);

                }

            }

            *size += (jsst + temp->data_length);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _jam_free_fields(jam_t *self, queue *fields) {

    int stat = OK;
    jam_field_t *temp = NULL;

    when_error_in {

        while ((temp = que_pop_head(fields))) {

            stat = self->_free_field(self, temp);
            check_return(stat, self);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _jam_free_field(jam_t *self, jam_field_t *field) {

    int stat = OK;

    free(field->buffer);
    free(field);
    
    return stat;

}

int _jam_new_field(jam_t *self, ushort item, char *buffer, jam_field_t **field) {

    int stat = OK;

    when_error_in {

        errno = 0;
        *field = calloc(1, sizeof(jam_field_t));
        if (*field == NULL) {

            cause_error(errno);

        }

        (**field).lo_id = item;
        (**field).hi_id = 0;
        (**field).data_length = strlen(buffer);
        (**field).buffer = strndup(buffer, strlen(buffer));

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _jam_find_text(jam_t *self, off_t offset, char **text, int size) {

    int stat = OK;

    when_error_in {

        stat = files_seek(self->text, offset, SEEK_SET);
        check_return(stat, self->text);

        stat = self->_read_text(self, text, size);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _jam_append_text(jam_t *self, char *text, int size, ssize_t *offset) {

    int stat = OK;

    when_error_in {

        stat = files_seek(self->text, 0, SEEK_END);
        check_return(stat, self->text);

        stat = files_tell(self->text, offset);
        check_return(stat, self->text);

        stat = self->_write_text(self, text, size);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _jam_write_text(jam_t *self, char *text, int size) {

    int stat = OK;
    ssize_t count = 0;

    when_error_in {

        stat = files_write(self->text, text, size, &count);
        check_return(stat, self->text);

        if (count != size) {

            cause_error(EIO);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _jam_read_text(jam_t *self, char **text, int size) {

    int stat = OK;
    ssize_t count = 0;

    when_error_in {

        stat = files_read(self->text, *text, size, &count);
        check_return(stat, self->text);

        if (count != size) {

            cause_error(EIO);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _jam_find_message(jam_t *self, off_t offset, jam_message_t *message, queue *fields) {

    int stat = OK;

    when_error_in {

        stat = files_seek(self->header, offset, SEEK_SET);
        check_return(stat, self->header);

        stat = self->_read_message(self, message, fields);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _jam_append_message(jam_t *self, jam_message_t *message, queue *fields, ssize_t *offset) {

    int stat = OK;

    when_error_in {

        stat = files_seek(self->header, 0, SEEK_END);
        check_return(stat, self->header);

        stat = files_tell(self->header, offset);
        check_return(stat, self->header);

        stat = self->_write_message(self, message, fields);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _jam_normalize_message(jam_t *self, jam_message_t *oldmsg, queue *oldfields, jam_message_t *message, queue *fields) {

    int stat = OK;

    if (oldmsg->times_read > message->times_read) {

        (*message).times_read = oldmsg->times_read;

    }

    if (oldmsg->reply_to != message->reply_to) {

        (*message).reply_to = oldmsg->reply_to;

    }

    if (oldmsg->reply_1st != message->reply_1st) {

        (*message).reply_1st = oldmsg->reply_1st;

    }

    if (oldmsg->reply_next != message->reply_next) {

        (*message).reply_next = oldmsg->reply_next;

    }

    if (oldmsg->date_received > message->date_received) {

        (*message).date_received = oldmsg->date_received;

    }

    if (oldmsg->date_processed > message->date_processed) {

        (*message).date_processed = oldmsg->date_processed;

    }

    if (oldmsg->attribute != message->attribute) {

        (*message).attribute = oldmsg->attribute;

    }

    if (oldmsg->password_crc != message->password_crc) {

        (*message).password_crc = oldmsg->password_crc;

    }

    if (oldmsg->cost != message->cost) {

        (*message).cost = oldmsg->cost;

    }

    return stat;

}

int _jam_read_message(jam_t *self, jam_message_t *message, queue *fields) {

    int stat = OK;
    int length = 0;
    ssize_t count = 0;

    when_error_in {

        stat = files_read(self->header, message, sizeof(jam_message_t), &count);
        check_return(stat, self->header);

        /* sanity checks */

        if (count != sizeof(jam_message_t)) {

            cause_error(EIO);

        }

        if (memcmp(message->signature, HEADERSIGNATURE, 4) != 0) {

            cause_error(EIO);

        }

        if (message->revision != 1) {

            cause_error(EIO);

        }

        /* get the subfields */

        length = message->subfield_length;

        if (length > 0) {

            stat = self->_read_fields(self, length, fields);
            check_return(stat, self);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _jam_write_message(jam_t *self, jam_message_t *message, queue *fields) {

    int size = 0;
    int stat = OK;
    ssize_t count = 0;

    when_error_in {

        stat = files_write(self->header, message, sizeof(jam_message_t), &count);
        check_return(stat, self->header);

        if (count != sizeof(jam_message_t)) {

            cause_error(EIO);

        }

        if (message->subfield_length > 0) {

            stat = self->_write_fields(self, fields, &size);
            check_return(stat, self);

            if (size != message->subfield_length) {

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

/* borrowed from jamlib */

int _jam_crc32(jam_t *self, uchar *buffer, ulong length, ulong *results) {

    static ulong crc32tab[] = {
        0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
        0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
        0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
        0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
        0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
        0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
        0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
        0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
        0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
        0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
        0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
        0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
        0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
        0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
        0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
        0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
        0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
        0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
        0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
        0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
        0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
        0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
        0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
        0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
        0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
        0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
        0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
        0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
        0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
        0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
        0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
        0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
    };

    int c;
    ulong crc = 0xffffffff;

    for (c = 0; c < length; c++) {

        crc = (crc >> 8) ^ crc32tab[(uchar)crc ^ tolower(buffer[c])];

    }

    *results = crc;

    return(OK);

}

