
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2019 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include "when.h"
#include "object.h"
#include "message.h"
#include "message_priv.h"
#include "error_codes.h"

require_klass(OBJECT_KLASS);

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _message_ctor(object_t *, item_list_t *);
int _message_dtor(object_t *);

int _message_compare(message_t *, message_t *);
int _message_override(message_t *, item_list_t *);
int _message_del(message_t *, int);
int _message_add(message_t *, int, char *);
int _message_set(message_t *, int, char *);
int _message_get(message_t *, int, char *, int);
int _message_load(message_t *, messages_t *, int);

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

declare_klass(MESSAGE_KLASS) {
    .size = KLASS_SIZE(message_t),
    .name = KLASS_NAME(message_t),
    .ctor = _message_ctor,
    .dtor = _message_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

message_t *message_create(messages_t *messages, int msg_size) {

    int stat = ERR;
    item_list_t items[2];
    message_t *self = NULL;

    if (messages != NULL) {

        SET_ITEM(items[0], MESSAGE_K_MESSAGES, messages, msg_size, NULL);
        SET_ITEM(items[1], 0, 0, 0, 0);

    } else {

        SET_ITEM(items[0], 0, 0, 0, 0);
        SET_ITEM(items[1], 0, 0, 0, 0);

    }

    self = (message_t *)object_create(MESSAGE_KLASS, items, &stat);

    return self;

}

int message_destroy(message_t *self) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            if (object_assert(self, message_t)) {

                stat = self->dtor(OBJECT(self));
                check_status(stat, E_INVOPS);

            } else {

                object_set_error(OBJECT(self), E_INVOBJ);

            }

        } else {

            object_set_error(OBJECT(self), E_INVPARM);

        }

        exit_when;

    } error_handler {

        stat = ERR;

        object_set_error2(OBJECT(self), trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int message_override(message_t *self, item_list_t *items) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            stat = self->_override(self, items);
            check_status(stat, E_INVOPS);

        } else {

            object_set_error(OBJECT(self), E_INVPARM);

        }

        exit_when;

    } error_handler {

        stat = ERR;

        object_set_error2(OBJECT(self), trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int message_compare(message_t *us, message_t *them) {

    int stat = OK;

    when_error {

        if (us != NULL) {

            if (object_assert(them, message_t)) {

                stat = us->_compare(us, them);
                check_status(stat, E_INVOPS);

            } else {

                object_set_error(OBJECT(us), E_INVOBJ);

            }

        } else {

            object_set_error(OBJECT(us), E_INVPARM);

        }

        exit_when;

    } error_handler {

        stat = ERR;

        object_set_error2(OBJECT(us), trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int message_add(message_t *self, int nemonic, char *message) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (nemonic != 0) && (message != NULL)) {

            stat = self->_add_message(self, nemonic, message);
            check_status(stat, E_INVOPS);

        } else {

            object_set_error(OBJECT(self), E_INVPARM);

        }

        exit_when;

    } error_handler {

        stat = ERR;

        object_set_error2(OBJECT(self), trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int message_set(message_t *self, int nemonic, char *text) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (nemonic != 0) && (text != NULL)) {

            stat = self->_set_message(self, nemonic, text);
            check_status(stat, E_INVOPS);

        } else {

            object_set_error(OBJECT(self), E_INVPARM);

        }

        exit_when;

    } error_handler {

        stat = ERR;

        object_set_error2(OBJECT(self), trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int message_get(message_t *self, int nemonic, char *buffer, int size) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (nemonic != 0) && (buffer != NULL)) {

            stat = self->_get_message(self, nemonic, buffer, size);
            check_status(stat, E_NODATA);

        } else {

            object_set_error(OBJECT(self), E_INVPARM);

        }

        exit_when;

    } error_handler {

        stat = ERR;

        object_set_error2(OBJECT(self), trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int message_remove(message_t *self, int nemonic) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (nemonic != 0)) {

            stat = self->_del_message(self, nemonic);
            check_status(stat, E_INVOPS);

        } else {

            object_set_error(OBJECT(self), E_INVPARM);

        }

        exit_when;

    } error_handler {

        stat = ERR;

        object_set_error2(OBJECT(self), trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int message_load(message_t *self, messages_t *messages, int size) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (messages != NULL)) {

            stat = self->_load_messages(self, messages, size);
            check_status(stat, E_INVOPS);

        } else {

            object_set_error(OBJECT(self), E_INVPARM);

        }

        exit_when;

    } error_handler {

        stat = ERR;

        object_set_error2(OBJECT(self), trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

int _message_ctor(object_t *object, item_list_t *items) {

    int stat = ERR;
    int msg_size = 0;
    message_t *self = NULL;
    messages_t *messages = NULL;

    if (object != NULL) {

        /* capture our items */

        if (items != NULL) {

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case MESSAGE_K_MESSAGES: {
                        messages = items[x].buffer_address;
                        msg_size = items[x].buffer_length;
                        break;
                    }
                }

            }

        }

        /* initilize our base klass here */


        /* initialize our derived klass here */

        self = MESSAGE(object);

        /* assign our methods here */

        self->ctor = _message_ctor;
        self->dtor = _message_dtor;
        self->_compare = _message_compare;
        self->_override = _message_override;
        self->_add_message = _message_add;
        self->_del_message = _message_del;
        self->_get_message = _message_get;
        self->_set_message = _message_set;
        self->_load_messages = _message_load;

        /* initialize internal variables here */

        que_init(&self->messages);

        /* load default messages */

        stat = self->_load_messages(self, defaults, sizeof(defaults));
        if (stat != OK) {

            object_set_error(OBJECT(self), E_NOLOAD);

        }

        /* load any user defined messages */

        if (messages != NULL) {

            stat = self->_load_messages(self, messages, msg_size);
            if (stat != OK) {

                object_set_error(OBJECT(self), E_NOLOAD);

            }

        }

    }

    return stat;

}

int _message_dtor(object_t *object) {

    int stat = OK;
    messages_t *message = NULL;
    message_t *self = MESSAGE(object);

    /* free local resources here */

    while ((message = que_pop_head(&self->messages))) {

        free(message->text);
        free(message);

    }

    que_init(&self->messages);

    /* walk the chain, freeing as we go */

    object_demote(object, object_t);
    object_destroy(object);

    return stat;

}

int _message_override(message_t *self, item_list_t *items) {

    int stat = ERR;

    if (items != NULL) {

        int x;
        for (x = 0;; x++) {

            if ((items[x].buffer_length == 0) &&
                (items[x].item_code == 0)) break;

            switch(items[x].item_code) {
                case MESSAGE_M_DESTRUCTOR: {
                    self->dtor = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case MESSAGE_M_ADD_MESSAGE: {
                    self->_add_message = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case MESSAGE_M_DEL_MESSAGE: {
                    self->_del_message = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case MESSAGE_M_GET_MESSAGE: {
                    self->_get_message = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case MESSAGE_M_SET_MESSAGE: {
                    self->_set_message = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case MESSAGE_M_LOAD_MESSAGES: {
                    self->_load_messages = items[x].buffer_address;
                    stat = OK;
                    break;
                }
            }

        }

    }

    return stat;

}

int _message_compare(message_t *self, message_t *other) {

    int stat = -1;

    if ((object_compare(OBJECT(self), OBJECT(other)) == 0) &&
        (self->ctor == other->ctor) &&
        (self->dtor == other->dtor) &&
        (self->_compare == other->_compare) &&
        (self->_override == other->_override)) {

        stat = 0;

    }

    return stat;

}

int _message_add(message_t *self, int nemonic, char *text) {

    int stat = ERR;
    messages_t *message = NULL;

    if ((message = calloc(1, sizeof(messages_t)))) {

        message->nemonic = nemonic;
        message->text = strdup(text);

        stat = que_push_tail(&self->messages, message);

    }

    return stat;

}

int _message_del(message_t *self, int nemonic) {

    int stat = ERR;
    messages_t *message = NULL;

    for (message = que_first(&self->messages);
         message != NULL;
         message = que_next(&self->messages)) {

        if (message->nemonic == nemonic) {

            message = que_delete(&self->messages);

            free(message->text);
            free(message);

            stat = OK;

            break;

        }

    }

    if (que_empty(&self->messages)) {

        que_init(&self->messages);

    }

    return stat;

}

int _message_get(message_t *self, int nemonic, char *buffer, int size) {

    int stat = ERR;
    messages_t *message = NULL;

    for (message = que_first(&self->messages);
         message != NULL;
         message = que_next(&self->messages)) {

        if (message->nemonic == nemonic) {

            strncpy(buffer, message->text, size);
            stat = OK;
            break;

        }

    }

    return stat;

}

int _message_set(message_t *self, int nemonic, char *text) {

    int stat = ERR;
    messages_t *message = NULL;

    for (message = que_first(&self->messages);
         message != NULL;
         message = que_next(&self->messages)) {

        if (message->nemonic == nemonic) {

            free(message->text);
            message->text = strdup(text);

            stat = que_put(&self->messages, message);
            
            break;

        }

    }

    return stat;

}

int _message_load(message_t *self, messages_t *messages, int size) {

    int x;
    int stat = ERR;
    int count = (size/sizeof(messages[0]));

    for (x = 0; x < count; x++) {

        stat = self->_add_message(self, messages[x].nemonic, messages[x].text);
        if (stat != OK) break;

    }
    
    return stat;
    
}

