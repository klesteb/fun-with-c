
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2020 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include "when.h"
#include "files.h"
#include "object.h"
#include "error_codes.h"

require_klass(OBJECT_KLASS);

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _files_ctor(object_t *, item_list_t *);
int _files_dtor(object_t *);

int _files_compare(files_t *, files_t *);
int _files_override(files_t *, item_list_t *);

int _files_open(files_t *, int, mode_t);
int _files_close(files_t *);
int _files_seek(files_t *, off_t, int);
int _files_tell(files_t *, off_t *);
int _files_read(files_t *, void *, size_t, int *);
int _files_write(files_t *, void *, size_t, int *);
int _files_gets(files_t *, char *, int);
int _files_puts(files_t *, char *);

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

declare_klass(FILES_KLASS) {
    .size = KLASS_SIZE(files_t),
    .name = KLASS_NAME(files_t),
    .ctor = _files_ctor,
    .dtor = _files_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

files_t *files_create(item_list_t *items) {

    int stat = ERR;
    files_t *self = NULL;

    self = (files_t *)object_create(FILES_KLASS, items, &stat);

    return self;

}

int files_destroy(files_t *self) {

    int stat = OK;

    when_error_in {

        if (self != NULL) {

            if (object_assert(self, files_t)) {

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

int files_override(files_t *self, item_list_t *items) {

    int stat = OK;

    when_error_in {

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

int files_compare(files_t *us, files_t *them) {

    int stat = OK;

    when_error_in {

        if (us != NULL) {

            if (object_assert(them, files_t)) {

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

int files_open(files_t *self, int flags, mode_t mode) {

    int stat = OK;

    when_error_in {

        if ((self != NULL)) {

            stat = self->_open(self, flags, mode);
            check_return(stat, self);

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

int files_close(files_t *self) {

    int stat = OK;

    when_error_in {

        if ((self != NULL)) {

            stat = self->_close(self);
            check_return(stat, self);

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

int files_seek(files_t *self, off_t offset, int whence) {
    
    int stat = OK;

    when_error_in {

        if ((self != NULL)) {

            stat = self->_seek(self, offset, whence);
            check_return(stat, self);

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

int files_tell(files_t *self, off_t *offset) {
    
    int stat = OK;

    when_error_in {

        if ((self != NULL) && (offset != NULL)) {

            stat = self->_tell(self, offset);
            check_return(stat, self);

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

int files_read(files_t *self, void *buffer, size_t size, int *count) {
    
    int stat = OK;

    when_error_in {

        if ((self != NULL) && (buffer != NULL) && (count != NULL)) {

            stat = self->_read(self, buffer, size, count);
            check_return(stat, self);

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

int files_write(files_t *self, void *buffer, size_t size, int *count) {
    
    int stat = OK;

    when_error_in {

        if ((self != NULL) && (buffer != NULL) && (count != NULL)) {

            stat = self->_write(self, buffer, size, count);
            check_return(stat, self);

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

int files_gets(files_t *self, char *buffer, int length) {
    
    int stat = OK;

    when_error_in {

        if ((self != NULL) && (buffer != NULL)) {

            stat = self->_gets(self, buffer, length);
            check_return(stat, self);

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

int files_puts(files_t *self, char *buffer) {
    
    int stat = OK;

    when_error_in {

        if ((self != NULL) && (buffer != NULL)) {

            stat = self->_puts(self, buffer);
            check_return(stat, self);

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

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

int _files_ctor(object_t *object, item_list_t *items) {

    int stat = ERR;
    char path[256];
    files_t *self = NULL;

    if (object != NULL) {

        /* capture our items */

        if (items != NULL) {

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case FILES_K_PATH: {
                        memcpy(path, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    /* case FILES_K_LOCKER: { */
                    /*     memcpy(&type,  */
                    /*            items[x].buffer_address,  */
                    /*            items[x].buffer_length); */
                    /*     break; */
                    /* } */
                }

            }

        }

        /* initilize our base klass here */

        object_set_error1(object, OK);

        /* initialize our derived klass here */

        self = FILES(object);

        /* assign our methods here */

        self->ctor = _files_ctor;
        self->dtor = _files_dtor;
        self->_compare = _files_compare;
        self->_override = _files_override;

        self->_open = _files_open;
        self->_close = _files_close;
        self->_read = _files_read;
        self->_write = _files_write;
        self->_seek = _files_seek;
        self->_tell = _files_tell;
        self->_gets = _files_gets;
        self->_puts = _files_puts;
        
        /* initialize internal variables here */
        
        stat = OK;

    }

    return stat;

}

int _files_dtor(object_t *object) {

    int stat = OK;

    /* free local resources here */


    /* walk the chain, freeing as we go */

    object_demote(object, object_t);
    object_destroy(object);

    return stat;

}

int _files_override(files_t *self, item_list_t *items) {

    int stat = ERR;

    if (items != NULL) {

        int x;
        for (x = 0;; x++) {

            if ((items[x].buffer_length == 0) &&
                (items[x].item_code == 0)) break;

            switch(items[x].item_code) {
                case FILES_M_DESTRUCTOR: {
                    self->dtor = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case FILES_M_OPEN: {
                    self->_open = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case FILES_M_CLOSE: {
                    self->_close = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case FILES_M_READ: {
                    self->_read = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case FILES_M_WRITE: {
                    self->_write = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case FILES_M_SEEK: {
                    self->_seek = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case FILES_M_TELL: {
                    self->_tell = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case FILES_M_GETS: {
                    self->_gets = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case FILES_M_PUTS: {
                    self->_puts = items[x].buffer_address;
                    stat = OK;
                    break;
                }
            }

        }

    }

    return stat;

}

int _files_compare(files_t *self, files_t *other) {

    int stat = ERR;

    if ((object_compare(OBJECT(self), OBJECT(other)) == 0) &&
        (self->ctor == other->ctor) &&
        (self->dtor == other->dtor) &&
        (self->_compare == other->_compare) &&
        (self->_override == other->_override) &&
        (self->_open == other->_open) &&
        (self->_close == other->_close) &&
        (self->_read == other->_read) &&
        (self->_write == other->_write) &&
        (self->_seek == other->_seek) &&
        (self->_tell == other->_tell) &&
        (self->_gets == other->_gets) &&
        (self->_puts == other->_puts)) {

        stat = OK;

    }

    return stat;

}

int _files_open(files_t *self, int flags, mode_t mode) {

    int stat = OK;

    return stat;

}

int _files_close(files_t *self) {

    int stat = OK;

    return stat;

}

int _files_seek(files_t *self, off_t offset, int whence) {

    int stat = OK;

    return stat;

}

int _files_tell(files_t *self, off_t *offset) {

    int stat = OK;

    return stat;

}

int _files_read(files_t *self, void *buffer, size_t size, int *count) {

    int stat = OK;

    return stat;

}

int _files_write(files_t *self, void *buffer, size_t size, int *count) {

    int stat = OK;

    return stat;

}

int _files_gets(files_t *self, char *buffer, int length) {

    int stat = OK;

    return stat;

}

int _files_puts(files_t *self, char *buffer) {

    int stat = OK;

    return stat;

}

