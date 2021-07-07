
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

#include "mbf.h"
#include "qwk.h"
#include "when.h"
#include "object.h"
#include "tracer.h"
#include "error_codes.h"
#include "misc/misc.h"

require_klass(OBJECT_KLASS);

/*----------------------------------------------------------------*/
/* internal data structures                                       */
/*----------------------------------------------------------------*/

typedef struct {           /* QWK Message header                         */
   char  Status;           /* message status byte                        */
   char  Number[7];        /* .QWK = message number, .REP = conference   */
   char  Date[8];          /* date in ascii           (mm-dd-yy)         */
   char  Time[5];          /* time in ascii           (hh:mm)            */
   char  To[25];           /* Person addressed to                        */
   char  From[25];         /* Person who wrote message                   */
   char  Subject[25];      /* Subject of message                         */
   char  Password[12];     /* password                                   */
   char  Reply[8];         /* message # this refers to                   */
   char  Records[6];       /* # of 128 byte recs in message, inc header  */
   char  Alive;            /* always 0xE1                                */
   char  Conference[2];    /* area number,            (intel short int)  */
   char  SeqNumber[2];     /* logical message number  (intel short int)  */
   char  NetTag[1];        /* space: no tag, "*": tag present ???        */
} hdr;

typedef struct {           /* <area>.ndx                                 */
    char Index[4];         /* MSBIN floating format                      */
    char Conference;       /* Conference number - should be ignored      */
} ndx;

union swapper {            /* easily swap the bytes from intel format    */
   char  cnum[2];
   short snum;
};

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _qwk_ctor(object_t *, item_list_t *);
int _qwk_dtor(object_t *);
int _qwk_compare(qwk_t *, qwk_t *);
int _qwk_override(qwk_t *, item_list_t *);

int _qwk_open(qwk_t *);
int _qwk_close(qwk_t *);
int _qwk_close_ndx(qwk_t *);
int _qwk_open_ndx(qwk_t *, char *);
int _qwk_free_text(qwk_t *, char *);
int _qwk_put_notice(qwk_t *, char *); 
int _qwk_get_notice(qwk_t *, char **); 
int _qwk_new_header(qwk_t *, qwk_header_t **);
int _qwk_get_control(qwk_t *, qwk_control_t *); 
int _qwk_put_control(qwk_t *, qwk_control_t *);
int _qwk_get_ndx(qwk_t *, qwk_ndx_t *, ssize_t *);
int _qwk_put_ndx(qwk_t *, qwk_ndx_t *, ssize_t *);
int _qwk_new_ndx(qwk_t *, ulong, uchar, qwk_ndx_t **);
int _qwk_get_message(qwk_t *, ulong, qwk_header_t *, char **);
int _qwk_put_message(qwk_t *, qwk_header_t *, char *, ulong *);

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

declare_klass(QWK_KLASS) {
    .size = KLASS_SIZE(qwk_t),
    .name = KLASS_NAME(qwk_t),
    .ctor = _qwk_ctor,
    .dtor = _qwk_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

qwk_t *qwk_create(char *path, int retries, int timeout, int rep, tracer_t *dump) {

    int stat = ERR;
    qwk_t *self = NULL;
    item_list_t items[6];

    SET_ITEM(items[0], QWK_K_PATH, path, strlen(path), NULL);
    SET_ITEM(items[1], QWK_K_REP, &rep, sizeof(int), NULL);
    SET_ITEM(items[2], QWK_K_RETRIES, &retries, sizeof(int), NULL);
    SET_ITEM(items[3], QWK_K_TIMEOUT, &timeout, sizeof(int), NULL);
    SET_ITEM(items[4], QWK_K_TRACE, dump, 0, NULL);
    SET_ITEM(items[5], 0, 0, 0, 0);

    self = (qwk_t *)object_create(QWK_KLASS, items, &stat);

    return self;

}

int qwk_destroy(qwk_t *self) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            if (object_assert(self, qwk_t)) {

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

int qwk_override(qwk_t *self, item_list_t *items) {

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

int qwk_compare(qwk_t *us, qwk_t *them) {

    int stat = OK;

    when_error {

        if (us != NULL) {

            if (object_assert(them, qwk_t)) {

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

char *qwk_version(qwk_t *self) {

    char *version = VERSION;

    return version;

}

int qwk_new_header(qwk_t *self, qwk_header_t **header) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (header == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_new_header(self, header);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int qwk_new_ndx(qwk_t *self, ulong record, uchar conference, qwk_ndx_t **ndx) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (*ndx == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_new_ndx(self, record, conference, ndx);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int qwk_free_text(qwk_t *self, char *text) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (text == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_free_text(self, text);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int qwk_get_notice(qwk_t *self, char **text) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (text == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_get_notice(self, text);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int qwk_put_notice(qwk_t *self, char *text) {

    int stat = OK;

    when_error {

        if ((self == NULL) || (text == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_put_notice(self, text);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int qwk_open(qwk_t *self) {

    int stat = OK;

    when_error {

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

int qwk_close(qwk_t *self) {

    int stat = OK;

    when_error {

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

int qwk_open_ndx(qwk_t *self, char *name) {

    int stat = OK;

    when_error {

        if ((self == NULL) || (name == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_open_ndx(self, name);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int qwk_get_ndx(qwk_t *self, qwk_ndx_t *ndx, ssize_t *count) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (ndx == NULL) || (count == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_get_ndx(self, ndx, count);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int qwk_put_ndx(qwk_t *self, qwk_ndx_t *ndx, ssize_t *count) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (ndx == NULL) || (count == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_put_ndx(self, ndx, count);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int qwk_close_ndx(qwk_t *self) {

    int stat = OK;

    when_error {

        if (self == NULL) {

            cause_error(E_INVPARM);

        }

        stat = self->_close_ndx(self);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int qwk_get_control(qwk_t *self, qwk_control_t *control) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (control == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_get_control(self, control);
        check_status(stat, OK, E_INVOPS);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int qwk_put_control(qwk_t *self, qwk_control_t *control) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (control == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_put_control(self, control);
        check_status(stat, OK, E_INVOPS);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int qwk_get_message(qwk_t *self, ulong record, qwk_header_t *header, char **text) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (header == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_get_message(self, record, header, text);
        check_status(stat, OK, E_INVOPS);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int qwk_put_message(qwk_t *self, qwk_header_t *header, char *text, ulong *record) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (header == NULL) || (text == NULL) || (record == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_put_message(self, header, text, record);
        check_status(stat, OK, E_INVOPS);

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

int _qwk_ctor(object_t *object, item_list_t *items) {

    char path[256];
    int stat = ERR;
    int rep = FALSE;
    int timeout = 1;
    int retries = 30;
    char control[1024];
    char messages[1024];
    qwk_t *self = NULL;
    tracer_t *dump = NULL;

    if (object != NULL) {

        memset(path, '\0', 256);
        memset(control, '\0', 1024);
        memset(messages, '\0', 1024);

        /* capture our items */

        if (items != NULL) {

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case QWK_K_PATH: {
                        memcpy(path, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case QWK_K_REP: {
                        memcpy(&rep, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case QWK_K_TIMEOUT: {
                        memcpy(&timeout, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case QWK_K_RETRIES: {
                        memcpy(&retries, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case QWK_K_TRACE: {
                        dump = items[x].buffer_address; 
                        break;
                    }
                }

            }

        }

        /* initilize our base klass here */

        object_set_error1(object, OK);

        /* initialize our derived klass here */

        self = QWK(object);

        /* assign our methods here */

        self->ctor      = _qwk_ctor;
        self->dtor      = _qwk_dtor;
        self->_compare  = _qwk_compare;
        self->_override = _qwk_override;

        self->_get_ndx     = _qwk_get_ndx;
        self->_put_ndx     = _qwk_put_ndx;
        self->_free_text   = _qwk_free_text;
        self->_put_notice  = _qwk_put_notice;
        self->_get_notice  = _qwk_get_notice; 
        self->_get_control = _qwk_get_control;
        self->_put_control = _qwk_put_control;
        self->_get_message = _qwk_get_message;
        self->_put_message = _qwk_put_message;
        self->_open_ndx    = _qwk_open_ndx;
        self->_close_ndx   = _qwk_close_ndx;
        self->_open        = _qwk_open;
        self->_close       = _qwk_close;
        self->_new_header  = _qwk_new_header;
        self->_new_ndx     = _qwk_new_ndx;

        /* initialize internal variables here */

        when_error_in {

            self->rep = rep;
            self->ndx = NULL;
            self->trace = dump;
            self->retries = retries;
            self->timeout = timeout;
            self->path = strdup(path);

            strcpy(control, fnm_build(1, FnmPath, "control", ".dat", path, NULL));
            self->control = files_create(control, retries, timeout);
            check_creation(self->control);

            strcpy(messages, fnm_build(1, FnmPath, "messages", ".dat", path, NULL));
            self->messages = files_create(messages, retries, timeout);
            check_creation(self->messages);

            exit_when;

        } use {

            stat = ERR;
            process_error(self);

        } end_when;

    }

    return stat;

}

int _qwk_dtor(object_t *object) {

    int stat = OK;
    qwk_t *self = QWK(object);

    /* free local resources here */

    free(self->path);

    /* walk the chain, freeing as we go */

    object_demote(object, object_t);
    object_destroy(object);

    return stat;

}

int _qwk_override(qwk_t *self, item_list_t *items) {

    int stat = ERR;

    if (items != NULL) {

        int x;
        for (x = 0;; x++) {

            if ((items[x].buffer_length == 0) &&
                (items[x].item_code == 0)) break;

            switch(items[x].item_code) {
                case QWK_M_DESTRUCTOR: {
                    self->dtor = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case QWK_M_FREE_TEXT: {
                    self->_free_text = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case QWK_M_PUT_NOTICE: {
                    self->_put_notice = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case QWK_M_GET_NOTICE: {
                    self->_get_notice = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case QWK_M_GET_CONTROL: {
                    self->_get_control = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case QWK_M_PUT_CONTROL: {
                    self->_put_control = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case QWK_M_GET_NDX: {
                    self->_get_ndx = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case QWK_M_PUT_NDX: {
                    self->_put_ndx = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case QWK_M_GET_MESSAGE: {
                    self->_get_message = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case QWK_M_PUT_MESSAGE: {
                    self->_put_message = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case QWK_M_OPEN_NDX: {
                    self->_open_ndx = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case QWK_M_CLOSE_NDX: {
                    self->_close_ndx = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case QWK_M_OPEN: {
                    self->_open = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case QWK_M_CLOSE: {
                    self->_close = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case QWK_M_NEW_HEADER: {
                    self->_new_header = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case QWK_M_NEW_NDX: {
                    self->_new_ndx = items[x].buffer_address;
                    stat = OK;
                    break;
                }
            }

        }

    }

    return stat;

}

int _qwk_compare(qwk_t *self, qwk_t *other) {

    int stat = ERR;

    if ((object_compare(OBJECT(self), OBJECT(other)) == 0) &&
        (self->ctor == other->ctor) &&
        (self->dtor == other->dtor) &&
        (self->_compare == other->_compare) &&
        (self->_override == other->_override) &&
        (self->_open == other->_open) &&
        (self->_close == other->_close) &&
        (self->_close_ndx == other->_close_ndx) &&
        (self->_open_ndx == other->_open_ndx) &&
        (self->_free_text == other->_free_text) &&
        (self->_put_notice == other->_put_notice) &&
        (self->_get_notice == other->_get_notice) &&
        (self->_get_control == other->_get_control) &&
        (self->_put_control == other->_put_control) &&
        (self->_get_ndx == other->_get_ndx) &&
        (self->_put_ndx == other->_put_ndx) &&
        (self->_get_message == other->_get_message) &&
        (self->_put_message == other->_put_message) &&
        (self->_new_header == other->_new_header) &&
        (self->_new_ndx == other->_new_ndx) &&
        (self->rep == other->rep) &&
        (self->retries == other->retries) &&
        (self->timeout == other->timeout) &&
        (self->path == other->path) &&
        (self->messages == other->messages) &&
        (self->control == other->control) &&
        (self->ndx == other->ndx) &&
        (self->trace == other->trace)) {

        stat = OK;

    }

    return stat;

}

int _qwk_open(qwk_t *self) {

    int stat = OK;
    int exists = 0;
    int flags = O_RDWR;
    int mode = (S_IRWXU | S_IRWXG);
    int create = (O_RDWR | O_CREAT);


    when_error_in {

        stat = files_exists(self->control, &exists);
        check_return(stat, self->control);

        if (exists) {

            stat = files_open(self->control, flags, 0);
            check_return(stat, self->control);

        } else {

            stat = files_open(self->control, create, mode);
            check_return(stat, self->control);

        }

        stat = files_exists(self->messages, &exists);
        check_return(stat, self->messages);

        if (exists) {

            stat = files_open(self->messages, flags, 0);
            check_return(stat, self->messages);

        } else {

            stat = files_open(self->messages, create, mode);
            check_return(stat, self->messages);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _qwk_close(qwk_t *self) {

    int stat = OK;


    when_error_in {

        stat = files_close(self->control);
        check_return(stat, self->control);

        stat = files_close(self->messages);
        check_return(stat, self->messages);

        if (self->ndx != NULL) {

            stat = files_close(self->ndx);
            check_return(stat, self->ndx);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _qwk_free_text(qwk_t *self, char *text) {
/*
 * Function: QwkFreeText.c
 * Version : 1.0
 * Created : 19-Jul-1994
 * Author  : Kevin Esteb
 *
 * Description:
 *
 *    This function will free the text buffer. Its not really needed.
 *    Just included for completeness.
 *
 * Modifications:
 *
 * Varaiables:
 */
 
/*
 * Main part of functon.
 */

    free(text);

    return OK;

}

int _qwk_get_message(qwk_t *self, ulong record, qwk_header_t *header, char **text) { 
/*
 * Function: QwkGetMsg.c
 * Version : v2.0
 * Created : 06-Jun-1992
 * Author  : Kevin Esteb
 *
 * Description:
 *
 *    This function will read in the QWK header and all the text for the
 *    message. It will return the text buffer along with the buffers size.
 *    When done with the buffer, it can be free()ed.
 *
 * Modifications:
 *
 * Varaiables:
 */
 
    int yy;
    hdr rec;
    int year;
    char buff[32];
    int stat = OK;
    int x, text_size;
    struct tm timbuf;
    off_t offset = 0;
    ssize_t count = 0;
    union swapper swap;
    char *buffer = NULL;

    when_error_in {

        offset = QWK_OFFSET(record);

        stat = files_seek(self->messages, offset, SEEK_SET);
        check_return(stat, self->messages);

        stat = files_read(self->messages, &rec, QWK_BLOCK_SIZE, &count);
        check_return(stat, self->messages);

        if (count != QWK_BLOCK_SIZE) {

            cause_error(EIO);

        }

        (*header).status = rec.Status;
   
        memset(buff, '\0', 32);
        strncpy(buff, rec.Number, 7);
        (*header).number = atol(buff);
   
        memset(buff, '\0', 32);
        strncpy(buff, rec.Date, 8);
        buff[2] = '\0';
        buff[5] = '\0';
   
        yy = atoi(&buff[6]);              /* set the proper year        */
        if (yy < 50) {                    /* this should be good until  */
                                          /* 2050 AD                    */
            year = yy + 2000;

        } else {

            year = yy + 1900;

        }

        timbuf.tm_isdst = -1;                 /* DST                    */
        timbuf.tm_mday  = 0;                  /* nop                    */
        timbuf.tm_yday  = 0;                  /* nop                    */
        timbuf.tm_year  = year - 1900;        /* years since 1900       */
        timbuf.tm_mon   = atoi(&buff[0]) - 1; /* month                  */
        timbuf.tm_wday  = atoi(&buff[3]) - 1; /* day                    */
   
        memset(buff, '\0', 32);
        strncpy(buff, rec.Time, 5);
        buff[2] = '\0';
   
        timbuf.tm_hour = atoi(&buff[0]);  /* hour                       */
        timbuf.tm_min  = atoi(&buff[3]);  /* minute                     */
        timbuf.tm_sec  = 0;               /* seconds                    */

        (*header).date_time = mktime(&timbuf); /* convert to unix format*/

        memset((*header).to, '\0', 26);
        strncpy((*header).to,       rec.To,       25);

        memset((*header).from, '\0', 26);
        strncpy((*header).from,     rec.From,     25);

        memset((*header).subject, '\0', 26);
        strncpy((*header).subject,  rec.Subject,  25);

        memset((*header).password, '\0', 13);
        strncpy((*header).password, rec.Password, 12);
   
        memset(buff, '\0', 32);
        strncpy(buff, rec.Reply, 8);
        (*header).reply = atol(buff);

        memset(buff, '\0', 32);
        strncpy(buff, rec.Records, 6);
        (*header).records = atol(buff) - 1L; /* exclude the header record */

        (*header).alive = QWK_ISALIVE(rec.Alive);
   
#if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)

        /* Swap the bytes from intel to motorola format.                */

        swap.cnum[0] = (rec.Conference[1] == ' ' ? 0 : rec.Conference[1]);
        swap.cnum[1] =  rec.Conference[0];
        (*header).conference = swap.snum;

        /* Swap the bytes from intel to motorola format.                */

        swap.cnum[0] = (rec.SeqNumber[1] == ' ' ? 0 : rec.SeqNumber[1]);
        swap.cnum[1] = (rec.SeqNumber[0] == ' ' ? 0 : rec.SeqNumber[0]);
        (*header).seq_number = swap.snum;
#else 
        (*header).conference = rec.Conference;
        (*header).seq_number = rec.SeqNumber;
#endif

        if (self->rep) {

            (*header).conference = header->number;
            (*header).number = 0;

        }

        strncpy((*header).net_tag, rec.NetTag, 1);

        text_size = QWK_BUF_SIZE(header->records);
   
        errno = 0;
        buffer = calloc(1, text_size);
        if (buffer == NULL) {

            cause_error(errno);

        }

        stat = files_read(self->messages, buffer, text_size, &count);
        check_return(stat, self->messages);

        if (count != text_size) {

            cause_error(EIO);

        }

        for (x = 0; x < text_size; x++) {

            if (buffer[x] == '\xe3') buffer[x] = '\n';

        }

        buffer[text_size] = '\0';
        *text = buffer;

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _qwk_put_message(qwk_t *self, qwk_header_t *header, char *text, ulong *record) {
/*
 * Function: QwkPutMsg.c
 * Version : v2.0
 * Created : 06-Jun-1992
 * Author  : Kevin Esteb
 *
 * Description:
 *
 *    Write out the QWK message header and return the offset within the
 *    message file. Remember that this needs to be 1 based and not 0 based 
 *    and is a multiple of QWK_BLOCK_SIZE. The text buffer also needs to be
 *    space padded to a multiple of QWK_BLOCK_SIZE. The rep flag is set if 
 *    this is a reply packet.
 *
 * Modifications:
 *
 * Varaiables:
 */

    hdr rec;
    int len, x, w;
    int stat = OK;
    struct tm *tm;
    ssize_t count;
    off_t offset = 0;
    char *junk = NULL;
    union swapper swap;
    int size = strlen(text);


    when_error_in {

        (*header).records = QWK_REC_CNT(size) + 1;

        rec.Status = header->status;

        if (self->rep) {

            sprintf(rec.Number, "%-7d", header->conference);

        } else {

            sprintf(rec.Number, "%-7ld", header->number);

        }

        tm = localtime(&header->date_time);

        sprintf(rec.Date, "%02u-%02u-%02u", tm->tm_mon, tm->tm_mday, tm->tm_year);
        sprintf(rec.Time, "%02u:%02u", tm->tm_hour, tm->tm_min);

        sprintf(rec.To,       "%-25.25s", header->to);
        sprintf(rec.From,     "%-25.25s", header->from);
        sprintf(rec.Subject,  "%-25.25s", header->subject);
        sprintf(rec.Password, "%-12.12s", header->password);
        sprintf(rec.Reply,    "%-8ld",    header->reply);
        sprintf(rec.Records,  "%-6ld",    header->records); 

        if (header->alive) {

            rec.Alive = 0xe1;

        } else {

            rec.Alive = 0xe2;

        }

#if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)

        /* Swap the bytes from motorola to intel format.                     */
   
        swap.snum = header->conference;
        rec.Conference[0] =  swap.cnum[1];
        rec.Conference[1] = (swap.cnum[0] == 0 ? ' ' : swap.cnum[0]);
   
        /* Swap the bytes from motorola to intel format.                     */
   
        swap.snum = header->seq_number;
        rec.SeqNumber[0] = (swap.cnum[1] == 0 ? ' ' : swap.cnum[1]);
        rec.SeqNumber[1] = (swap.cnum[0] == 0 ? ' ' : swap.cnum[0]);
#else
        rec.Conference = header->conference;
        rec.SeqNumber = header->seq_number;
#endif

        strncpy(rec.NetTag, header->net_tag, 1);

        stat = files_tell(self->messages, &offset);
        check_return(stat, self->messages);

        *record = QWK_RECORD(offset);

        stat = files_write(self->messages, &rec, QWK_BLOCK_SIZE, &count);
        check_return(stat, self->messages);

        if (count != QWK_BLOCK_SIZE) {

            cause_error(EIO);

        }

        /* Change all the \n's to \xe3's.                                */
   
        for (x = 0; x < size; x++) {

            if (text[x] == '\n') text[x] = '\xe3';

        }

        /* The text blocks must be a multiple of QWK_BLOCK_SIZE.         */

        stat = files_write(self->messages, text, size, &count);
        check_return(stat, self->messages);

        /* pad the text, if needed */

        len = QWK_REC_CNT(count);
        w = ((len * QWK_BLOCK_SIZE) - count);

        if (w > 0) {

            junk = spaces(w);

            stat = files_write(self->messages, junk, strlen(junk), &count);
            check_return(stat, self->messages);

            if (count != strlen(junk)) {

                cause_error(EIO);

            }

            free(junk);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _qwk_get_notice(qwk_t *self, char **notice) {
 
    int stat = OK;
    ssize_t count = 0;


    when_error_in {

        /* The text blocks must be a multiple of QWK_BLOCK_SIZE.         */

        errno = 0;
        *notice = calloc(1, QWK_BLOCK_SIZE + 1);
        if (*notice == NULL) {
            
            cause_error(errno);
            
        }
        
        /* notices are the first record in the file */

        stat = files_seek(self->messages, 0, SEEK_SET);
        check_return(stat, self->messages);

        /* read the notice */

        stat = files_read(self->messages, *notice, QWK_BLOCK_SIZE, &count);
        check_return(stat, self->messages);

        if (count != QWK_BLOCK_SIZE) {

            cause_error(EIO);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _qwk_put_notice(qwk_t *self, char *notice) {
/*
 * Function: QwkNotice.c
 * Version : v2.0
 * Created : 20-Jul-1994
 * Author  : Kevin Esteb
 *
 * Description:
 *
 *    Write out the notice record. This is the first record in the QWK
 *    message file. The format is not important, but it needs to be 
 *    QWK_BLOCK_SIZE size.
 *
 * Modifications:
 *
 * Varaiables:
 */
 
    int stat = OK;
    ssize_t count = 0;
    char buffer[QWK_BLOCK_SIZE + 1];


    when_error_in {

        /* The text blocks must be a multiple of QWK_BLOCK_SIZE.         */

        memset(buffer, '\0', QWK_BLOCK_SIZE + 1);
        snprintf(buffer, QWK_BLOCK_SIZE, "%-128s", notice);

        stat = files_seek(self->messages, 0, SEEK_SET);
        check_return(stat, self->messages);

        stat = files_write(self->messages, buffer, QWK_BLOCK_SIZE, &count);
        check_return(stat, self->messages);

        if (count != QWK_BLOCK_SIZE) {

            cause_error(EIO);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _qwk_get_control(qwk_t *self, qwk_control_t *ctrl) {
/*
 * Function: QwkGetControl.c
 * Version : 1.0
 * Created : 25-Jul-1994
 * Author  : Kevin Esteb
 *
 * Description:
 *
 *    This function will read in the control.dat file.
 *
 * Modifications:
 *
 * Varaiables:
 */
 
    int x;
    char *t;
    int stat = OK;
    struct tm timbuf;
    char buff[BUFSIZ];
    ssize_t count = 0;
    qwk_area_t *area = NULL;


    when_error_in {

        stat = que_init(&ctrl->areas);
        check_status(stat, QUE_OK, E_INVOPS);

        /* Line #1, the name of the BBS you got this mail packet from.   */

        memset(buff, '\0', BUFSIZ);
        stat = files_gets(self->control, buff, BUFSIZ, &count);
        check_return(stat, self->control);
        if (count == 0) cause_error(EIO);

        stripcr(buff);
        strncpy((*ctrl).bbs_name, buff, 32);
        
        /* Line #2, The City and State where the BBS is located.         */
   
        memset(buff, '\0', BUFSIZ);
        stat = files_gets(self->control, buff, BUFSIZ, &count);
        check_return(stat, self->control);
        if (count == 0) cause_error(EIO);
   
        stripcr(buff);
        if (strchr(buff, ',')) {

            t = strtok(buff, ", ");
            strncpy((*ctrl).city, t, 32);

            t = strtok(NULL, "\n");
            strncpy((*ctrl).state, trim(t), 32);

        } else {

            strncpy((*ctrl).city, buff, 32);
            memset((*ctrl).state, ' ', 32);

        }

        /* Line #3, The BBS's phone number.                              */
   
        memset(buff, '\0', BUFSIZ);
        stat = files_gets(self->control, buff, BUFSIZ, &count);
        check_return(stat, self->control);
        if (count == 0) cause_error(EIO);
   
        stripcr(buff);
        strncpy((*ctrl).phone, buff, 16);

        /* Line #4, The Sysop's name.                                    */
   
        memset(buff, '\0', BUFSIZ);
        stat = files_gets(self->control, buff, BUFSIZ, &count);
        check_return(stat, self->control);
        if (count == 0) cause_error(EIO);
   
        stripcr(buff);

        t = strtok(buff, ",\n");
        strncpy((*ctrl).sysop, t, 31);

        /* Line #5, the serial number and BBS id.                        */

        memset(buff, '\0', BUFSIZ);
        stat = files_gets(self->control, buff, BUFSIZ, &count);
        check_return(stat, self->control);
        if (count == 0) cause_error(EIO);

        stripcr(buff);

        t = strtok(buff, ", ");
        strncpy((*ctrl).serial_num, t, 32);

        t = strtok(NULL, "\n");
        strncpy((*ctrl).bbs_id, t, 32);

        /* Line #6, Time and Date of packet, changed to Unix format.     */

        memset(buff, '\0', BUFSIZ);
        stat = files_gets(self->control, buff, BUFSIZ, &count);
        check_return(stat, self->control);
        if (count == 0) cause_error(EIO);

        stripcr(buff);

        buff[2]  = '\0';
        buff[5]  = '\0';
        buff[10] = '\0';
        buff[13] = '\0';
        buff[16] = '\0';

        timbuf.tm_isdst = -1;                   /* DST                    */
        timbuf.tm_mday  = 0;                    /* nop                    */
        timbuf.tm_yday  = 0;                    /* nop                    */
        timbuf.tm_year = atoi(&buff[6]) - 1900; /* years since 1900       */
        timbuf.tm_mon  = atoi(&buff[0]) - 1;    /* month                  */
        timbuf.tm_wday = atoi(&buff[3]) - 1;    /* day                    */
        timbuf.tm_hour = atoi(&buff[11]);       /* hours                  */
        timbuf.tm_min  = atoi(&buff[14]);       /* minutes                */
        timbuf.tm_sec  = atoi(&buff[17]);       /* seconds                */

        (*ctrl).date_time = mktime(&timbuf);

        /* Line #7, Uppercase name of user, this packet was prepared for. */
   
        memset(buff, '\0', BUFSIZ);
        stat = files_gets(self->control, buff, BUFSIZ, &count);
        check_return(stat, self->control);
        if (count == 0) cause_error(EIO);
   
        stripcr(buff);
        strncpy((*ctrl).username, buff, 32);

        /* Line #8, Name of the Menu file, mostly for QMail reader/door. */
   
        memset(buff, '\0', BUFSIZ);
        stat = files_gets(self->control, buff, BUFSIZ, &count);
        check_return(stat, self->control);
        if (count == 0) cause_error(EIO);

        /* Line #9, Who knows, usually 0.                                */
   
        memset(buff, '\0', BUFSIZ);
        stat = files_gets(self->control, buff, BUFSIZ, &count);
        check_return(stat, self->control);
        if (count == 0) cause_error(EIO);

        /* Line #10, Who knows, usually 0.                               */
   
        memset(buff, '\0', BUFSIZ);
        stat = files_gets(self->control, buff, BUFSIZ, &count);
        check_return(stat, self->control);
        if (count == 0) cause_error(EIO);

        /* Line #11, maxinum number of conferences, PLUS 1.              */
   
        memset(buff, '\0', BUFSIZ);
        stat = files_gets(self->control, buff, BUFSIZ, &count);
        check_return(stat, self->control);
        if (count == 0) cause_error(EIO);
   
        stripcr(buff);
        (*ctrl).num_areas = atol(buff) + 1;

        /* Allocate the memory and read in the Area numbers and names.   */
        /* These are locigal lines 12 and 13.                            */

        for (x = 0; x < ctrl->num_areas; x++) {

            errno = 0;
            area = calloc(1, sizeof(qwk_area_t));
            if (area == NULL) {

                cause_error(errno);

            }

            memset(buff, '\0', BUFSIZ);
            stat = files_gets(self->control, buff, BUFSIZ, &count);
            check_return(stat, self->control);
            if (count == 0) cause_error(EIO);

            stripcr(buff);
            area->area = atol(buff);

            memset(buff, '\0', BUFSIZ);
            stat = files_gets(self->control, buff, BUFSIZ, &count);
            check_return(stat, self->control);
            if (count == 0) cause_error(EIO);

            stripcr(buff);
            strncpy(area->name, buff, 32);

            stat = que_push_head(&ctrl->areas, area);
            check_status(stat, QUE_OK, E_NOQUEUE);

        }

        /* The hello, news and goodbye files are optional.               */
   
        memset(buff, '\0', BUFSIZ);
        stat = files_gets(self->control, buff, BUFSIZ, &count);
        check_return(stat, self->control);
        if (count == 0) exit_when;
   
        stripcr(buff);
        strncpy((*ctrl).hello_file, buff, 32);

        memset(buff, '\0', BUFSIZ);
        stat = files_gets(self->control, buff, BUFSIZ, &count);
        check_return(stat, self->control);
        if (count == 0) exit_when;
   
        stripcr(buff);
        strncpy((*ctrl).news_file, buff, 32);

        memset(buff, '\0', BUFSIZ);
        stat = files_gets(self->control, buff, BUFSIZ, &count);
        check_return(stat, self->control);
        if (count == 0) exit_when;
   
        stripcr(buff);
        strncpy((*ctrl).goodbye_file, buff, 32);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _qwk_put_control(qwk_t *self, qwk_control_t *ctrl) { 
/*
 * Function: QwkPutControl.c
 * Version : 1.0
 * Created : 25-Jul-1994
 * Author  : Kevin Esteb
 *
 * Description:
 *
 *    This file will write out a control.dat file.
 *
 * Modifications:
 *
 * Varaiables:
 */

    char buff[80];
    struct tm *tm;
    int stat = OK;
    ssize_t count = 0;
    qwk_area_t *area = NULL;


    when_error_in {

        stat = files_set_eol(self->control, "\r\n");
        check_return(stat, self->control);

        /* Line #1, the bbs's name.                                      */
   
        stat = files_puts(self->control, ctrl->bbs_name, &count);
        check_return(stat, self->control);
        if (count == 0) cause_error(EIO);

        /* Line #2, the bbs's location.                                  */
   
        memset(buff, '\0', 80);
        snprintf(buff, 79, "%s, %s", ctrl->city, ctrl->state);
        stat = files_puts(self->control, buff, &count);
        check_return(stat, self->control);
        if (count == 0) cause_error(EIO);

        /* Line #3, the bbs's phone number.                              */
   
        stat = files_puts(self->control, ctrl->phone, &count);
        check_return(stat, self->control);
        if (count == 0) cause_error(EIO);
   
        /* Line #4, the sysop's name.                                    */
   
        memset(buff, '\0', 80);
        snprintf(buff, 79, "%s, Sysop", ctrl->sysop);
        stat = files_puts(self->control, buff, &count);
        check_return(stat, self->control);
        if (count == 0) cause_error(EIO);

        /* Line #5, the serial number of the mail door and bbs id.       */

        memset(buff, '\0', 80);
        snprintf(buff, 79, "%s,%s", ctrl->serial_num, ctrl->bbs_id);
        stat = files_puts(self->control, buff, &count);
        check_return(stat, self->control);
        if (count == 0) cause_error(EIO);
   
        /* Line #6, date and time of this packet.                        */
   
        memset(buff, '\0', 80);
        tm = localtime(&ctrl->date_time);
        sprintf(buff, "%02d-%02d-%04d,%02d:%02d:%02d",
                tm->tm_mon + 1, tm->tm_mday, 1900 + tm->tm_year,
                tm->tm_hour, tm->tm_min, tm->tm_sec);
        stat = files_puts(self->control, buff, &count);
        check_return(stat, self->control);
        if (count == 0) cause_error(EIO);
   
        /* Line #7, Upper case name of the user.                         */
   
        strupr(ctrl->username);
        stat = files_puts(self->control, ctrl->username, &count);
        check_return(stat, self->control);
        if (count == 0) cause_error(EIO);
   
        /* Line #8, the menu file, mostly for QMail reader/door.         */
   
        stat = files_puts(self->control, "", &count);
        check_return(stat, self->control);
        if (count == 0) cause_error(EIO);

        /* Line #9, unknow line, usually zero.                           */
   
        stat = files_puts(self->control, "0", &count);
        check_return(stat, self->control);
        if (count == 0) cause_error(EIO);
   
        /* Line #10, total number of messages in packet.                 */
   
        stat = files_puts(self->control, "0", &count);
        check_return(stat, self->control);
        if (count == 0) cause_error(EIO);
   
        /* Line #11, maxinum number of conferences, MINUS 1.             */
   
        memset(buff, '\0', 80);
        snprintf(buff, 79, "%ld", ctrl->num_areas - 1);
        stat = files_puts(self->control, buff, &count);
        check_return(stat, self->control);
        if (count == 0) cause_error(EIO);
  
        /* Line #12, the conference number.                              */
        /* Line #13, the conference name, limited to 10 chars or less.   */
   
        for (area = que_first(&ctrl->areas);
             area != NULL;
             area = que_next(&ctrl->areas)) {

            memset(buff, '\0', 80);
            snprintf(buff, 79, "%ld", area->area);
            stat = files_puts(self->control, buff, &count);
            check_return(stat, self->control);
            if (count == 0) cause_error(EIO);

            memset(buff, '\0', 80);
            snprintf(buff, 11, "%s", area->name);
            stat = files_puts(self->control, buff, &count);
            check_return(stat, self->control);
            if (count == 0) cause_error(EIO);

        }
   
        /* Line #14, the hello file to be displayed.          (optional) */
   
        stat = files_puts(self->control, ctrl->hello_file, &count);
        check_return(stat, self->control);
        if (count == 0) cause_error(EIO);

        /* Line #15, the news file to be displayed.           (optional) */

        stat = files_puts(self->control, ctrl->news_file, &count);
        check_return(stat, self->control);
        if (count == 0) cause_error(EIO);

        /* Line #16, the goodbye file to be displayed.        (optional) */

        stat = files_puts(self->control, ctrl->goodbye_file, &count);
        check_return(stat, self->control);
        if (count == 0) cause_error(EIO);

        stat = files_set_eol(self->control, "\n");
        check_return(stat, self->control);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _qwk_open_ndx(qwk_t *self, char *name) {

    int stat = OK;
    int exists = 0;
    char index[1024];
    int flags = O_RDWR;
    int create = (O_RDWR | O_CREAT);
    int mode = (S_IRWXU | S_IRWXG);


    when_error_in {

        memset(index, '\0', 1024);
        strcpy(index, fnm_build(1, FnmPath, name, ".ndx", self->path, NULL));
        self->ndx = files_create(index, self->retries, self->timeout);
        check_creation(self->ndx);

        stat = files_exists(self->ndx, &exists);
        check_return(stat, self->ndx);

        if (exists) {

            stat = files_open(self->ndx, flags, 0);
            check_return(stat, self->ndx);

        } else {

            stat = files_open(self->ndx, create, mode);
            check_return(stat, self->ndx);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _qwk_close_ndx(qwk_t *self) {

    int stat = OK;

    when_error_in {

        if (self->ndx != NULL) {

            stat = files_close(self->ndx);
            check_return(stat, self->ndx);

            self->ndx = NULL;

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _qwk_get_ndx(qwk_t *self, qwk_ndx_t *n, ssize_t *count) {
/*
 * Function: QwkGetNdx.c
 * Version : v2.0
 * Created : 06-Jun-1992
 * Author  : Kevin Esteb
 *
 * Description:
 *
 *    Read QWK index record, convert nasty MSBIN to long.
 *
 * Modifications:
 *
 * Varaiables:
 */
 
    ndx ndx;
    int stat = OK;
    long index = 0;

    union {
        msbin msbin;
        char buff[4];
    } join;

    when_error_in {

        stat = files_read(self->ndx, &ndx, sizeof(ndx), count);
        check_return(stat, self->ndx);

        if (*count == sizeof(ndx)) {

            memcpy(&join.buff, ndx.Index, 4);
            MbfToLong(&join.msbin, &index);

            (*n).index = index;
            (*n).conference = ndx.Conference;

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _qwk_put_ndx(qwk_t *self, qwk_ndx_t *n, ssize_t *count) { 
/*
 * Function: QwkPutNdx.c
 * Version : v2.0
 * Created : 06-Jun-1992
 * Author  : Kevin Esteb
 *
 * Description:
 *
 *    Write QWK index record, converting long to nasty MSBIN.
 *
 * Modifications:
 *
 * Varaiables:
 */
 
    ndx ndx;
    msbin junk;
    int stat = OK;

    when_error_in {

        LongToMbf(n->index, &junk);
        memcpy(&ndx.Index, &junk, 4);
        ndx.Conference = n->conference;

        stat = files_write(self->ndx, &ndx, sizeof(ndx), count);
        check_return(stat, self->ndx);

        if (*count != sizeof(ndx)) {

            cause_error(EIO);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _qwk_new_header(qwk_t *self, qwk_header_t **header) {

    int stat = OK;

    when_error_in {

        errno = 0;
        *header = calloc(1, sizeof(qwk_header_t));
        if (*header == NULL) {

            cause_error(errno);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _qwk_new_ndx(qwk_t *self, ulong record, uchar conference, qwk_ndx_t **ndx) {

    int stat = OK;

    when_error_in {

        errno = 0;
        *ndx = calloc(1, sizeof(qwk_ndx_t));
        if (*ndx == NULL) {

            cause_error(errno);

        }

        (**ndx).index = record;
        (**ndx).conference = conference;

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

