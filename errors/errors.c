
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

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "object.h"
#include "errors.h"
#include "errors_priv.h"

require_klass(OBJECT_KLASS);

/*----------------------------------------------------------------*/
/* klass defination                                               */
/*----------------------------------------------------------------*/

int _errors_ctor(object_t *object, item_list_t *);
int _errors_dtor(object_t *);

char *_errors_get_text(errors_t *, int);
char *_errors_get_message(errors_t *, int);
int _errors_remove_error_code(errors_t *, int);
int _errors_add_error_code(errors_t *, int, char *, char *);
int _errors_add_error_codes(errors_t *, error_code_t *, int);

declare_klass(ERRORS_KLASS) {
    .size = KLASS_SIZE(errors_t),
    .name = KLASS_NAME(errors_t),
    .ctor = _errors_ctor,
    .dtor = _errors_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

errors_t *errors_create(void) {

    int stat = ERR;
    errors_t *self = NULL;

    self = (errors_t *)object_create(ERRORS_KLASS, NULL, &stat);

    return self;

}

int errors_destroy(errors_t *self) {

    int stat = ERR;

    if (self != NULL) {

        if (object_assert(self, errors_t)) {

            stat = self->dtor(OBJECT(self));

        }

    }

    return stat;

}

char *errors_get_text(errors_t *self, int ec) {

    char *buffer = NULL;

    if (self != NULL) {

        buffer = self->_get_text(self, ec);

    }

    return buffer;
    
}

char *errors_get_message(errors_t *self, int ec) {

    char *buffer = NULL;
    
    if (self != NULL) {

        buffer = self->_get_message(self, ec);

    }

    return buffer;
    
}

int errors_add_error_code(errors_t *self, int ec, char *text, char *message) {

    int stat = ERR;

    if (self != NULL) {

        stat = self->_add_error_code(self, ec, text, message);

    }

    return stat;

}

int errors_remove_error_code(errors_t *self, int ec) {

    int stat = ERR;

    if (self != NULL) {

        stat = self->_remove_error_code(self, ec);

    }

    return stat;

}

int errors_add_error_codes(errors_t *self, error_code_t *codes, int size) {

    int stat = ERR;

    if ((self != NULL) && (codes != NULL)) {

        stat = self->_add_error_codes(self, codes, size);

    }

    return stat;

}

/*----------------------------------------------------------------*/
/* private methods                                                */
/*----------------------------------------------------------------*/

static void _load_system_error_codes(errors_t *);

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

int _errors_ctor(object_t *object, item_list_t *items) {

    int stat = ERR;
    int codes_size = 0;
    error_code_t *codes;
    errors_t *self = NULL;

    if (object != NULL) {

        /* capture our items */

        if (items != NULL) {

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case ERRORS_K_ERROR_CODES: {
                        codes = items[x].buffer_address;
                        codes_size = items[x].buffer_length;
                        break;
                    }

                }

            }

        }

        /* initilize our base klass here */

        /* initialize our derived klass here */

        self = ERRORS(object);

        /* assign our methods here */

        self->ctor = _errors_ctor;
        self->dtor = _errors_dtor;

        self->_get_text = _errors_get_text;
        self->_get_message = _errors_get_message;
        self->_add_error_code = _errors_add_error_code;
        self->_add_error_codes = _errors_add_error_codes;
        self->_remove_error_code = _errors_remove_error_code;

        /* initialize internal variables here */

        que_init(&self->error_codes);

        /* load the system error codes */

        _load_system_error_codes(self);

        /* load our error codes */

        self->_add_error_codes(self, error_codes, sizeof(error_codes));

        stat = OK;

    }

    return stat;

}

int _errors_dtor(object_t *object) {

    int stat = OK;
    errors_t *self = ERRORS(object);
    error_code_t *error_code = NULL;

    /* free local resources here */

    while ((error_code = que_pop_head(&self->error_codes))) {

        free(error_code->text);
        free(error_code->message);
        free(error_code);

    }

    que_init(&self->error_codes);

    /* walk the chain, freeing as we go */

    object_demote(object, object_t);
    object_destroy(object);

    return stat;

}

char *_errors_get_text(errors_t *self, int ec) {

    char *buffer = "Unknown error";
    error_code_t *error_code = NULL;

    for (error_code = que_first(&self->error_codes);
         error_code != NULL;
         error_code = que_next(&self->error_codes)) {

        if (error_code->ec == ec) {

            buffer = error_code->text;
            break;

        }

    }

    return buffer;

}

char *_errors_get_message(errors_t *self, int ec) {

    char *buffer = "Unknown error";
    error_code_t *error_code = NULL;

    for (error_code = que_first(&self->error_codes);
         error_code != NULL;
         error_code = que_next(&self->error_codes)) {

        if (error_code->ec == ec) {

            buffer = error_code->message;
            break;

        }

    }

    return buffer;

}

int _errors_add_error_code(errors_t *self, int ec, char *text, char *message) {

    int stat = ERR;
    error_code_t *error_code = NULL;

    if ((error_code = calloc(1, sizeof(error_code_t)))) {

        error_code->ec = ec;
        error_code->text = strdup(text);
        error_code->message = strdup(message);

        stat = que_push_tail(&self->error_codes, error_code);

    }

    return stat;

}

int _errors_remove_error_code(errors_t *self, int ec) {

    int stat = ERR;
    error_code_t *error_code = NULL;

    for (error_code = que_first(&self->error_codes);
         error_code != NULL;
         error_code = que_next(&self->error_codes)) {

        if (error_code->ec == ec) {

            error_code = que_delete(&self->error_codes);

            free(error_code->text);
            free(error_code->message);
            free(error_code);
            
            stat = OK;

            break;

        }

    }

    if (que_empty(&self->error_codes)) {

        que_init(&self->error_codes);

    }

    return stat;

}

int _errors_add_error_codes(errors_t *self, error_code_t *codes, int size) {

    int x;
    int stat = ERR;
    int count = (size/sizeof(codes[0]));

    for (x = 0; x < count; x++) {

        stat = self->_add_error_code(self, codes[x].ec, codes[x].text, codes[x].message);
        if (stat != OK) break;

    }
    
    return stat;
    
}

/*----------------------------------------------------------------*/
/* private methods                                                */
/*----------------------------------------------------------------*/

static void _load_system_error_codes(errors_t *self) {

    /*                                                            */
    /* concept and macro magic taken from:                        */
    /*                                                            */
    /* https://github.com/mentalisttraceur/errnoname/errnoname.c  */
    /*                                                            */
    /* load the system error codes and associated error message.  */
    /*                                                            */

#ifdef E2BIG
        self->_add_error_code(self, E2BIG, "E2BIG", strerror(E2BIG));
#endif
#ifdef EACCES
        self->_add_error_code(self, EACCES, "EACCES", strerror(EACCES));
#endif
#ifdef EADDRINUSE
        self->_add_error_code(self, EADDRINUSE, "EADDRINUSE", strerror(EADDRINUSE));
#endif
#ifdef EADDRNOTAVAIL
        self->_add_error_code(self, EADDRNOTAVAIL, "EADDRNOTAVAIL", strerror(EADDRNOTAVAIL));
#endif
#ifdef EADI
        self->_add_error_code(self, EADI, "EADI", strerror(EADI));
#endif
#ifdef EADV
        self->_add_error_code(self, EADV, "EADV", strerror(EADV));
#endif
#ifdef EAFNOSUPPORT
        self->_add_error_code(self, EAFNOSUPPORT, "EAFNOSUPPORT", strerror(EAFNOSUPPORT));
#endif
#ifdef EAGAIN
        self->_add_error_code(self, EAGAIN, "EAGAIN", strerror(EAGAIN));
#endif
#ifdef EAIO
        self->_add_error_code(self,EAIO , "EAIO", strerror(EAIO));
#endif
#ifdef EALIGN
        self->_add_error_code(self, EALIGN , "EALIGN", strerror(EALIGN));
#endif
#ifdef EALREADY
        self->_add_error_code(self, EALREADY, "EALREADY", strerror(EALREADY));
#endif
#ifdef EASYNC
        self->_add_error_code(self, EASYNC, "EASYNC", strerror(EASYNC));
#endif
#ifdef EAUTH
        self->_add_error_code(self, EAUTH, "EAUTH", strerror());
#endif
#ifdef EBADARCH
        self->_add_error_code(self, EBADARCH, "EBADARCH", strerror(EBADARCH));
#endif
#ifdef EBADE
        self->_add_error_code(self, EBADE, "EBADE", strerror(EBADE));
#endif
#ifdef EBADEXEC
        self->_add_error_code(self, EBADEXEC, "EBADEXEC", strerror(EBADEXEC));
#endif
#ifdef EBADF
        self->_add_error_code(self, EBADF, "EBADF", strerror(EBADF));
#endif
#ifdef EBADFD
        self->_add_error_code(self, EBADFD, "EBADFD", strerror(EBADFD));
#endif
#ifdef EBADMACHO
        self->_add_error_code(self, EBADMACHO, "EBADMACHO", strerror(EBADMACHO));
#endif
#ifdef EBADMSG
        self->_add_error_code(self, EBADMSG, "EBADMSG", strerror(EBADMSG));
#endif
#ifdef EBADR
        self->_add_error_code(self, EBADR, "EBADR", strerror(EBADR));
#endif
#ifdef EBADRPC
        self->_add_error_code(self, EBADRPC, "EBADRPC", strerror(EBADRPC));
#endif
#ifdef EBADRQC
        self->_add_error_code(self, EBADRQC, "EBADRQC", strerror(EBADRQC));
#endif
#ifdef EBADSLT
        self->_add_error_code(self, EBADSLT, "EBADSLT", strerror(EBADSLT));
#endif
#ifdef EBADVER
        self->_add_error_code(self, EBADVER, "EBADVER", strerror(EBADVER));
#endif
#ifdef EBFONT
        self->_add_error_code(self, EBFONT, "EBFONT", strerror(EBFONT));
#endif
#ifdef EBUSY
        self->_add_error_code(self, EBUSY, "EBUSY", strerror(EBUSY));
#endif
#ifdef ECANCELED
        self->_add_error_code(self, ECANCELED, "ECANCELED", strerror(ECANCELED));
#endif
#if defined(ECANCELLED) && (!defined(ECANCELED) || ECANCELLED != ECANCELED)
        self->_add_error_code(self, ECANCELLED, "ECANCELLED", strerror(ECANCELLED));
#endif
#ifdef ECAPMODE
        self->_add_error_code(self, ECAPMODE, "ECAPMODE", strerror(ECAPMODE));
#endif
#ifdef ECHILD
        self->_add_error_code(self, ECHILD, "ECHILD", strerror(ECHILD));
#endif
#ifdef ECHRNG
        self->_add_error_code(self, ECHRNG, "ECHRNG", strerror(ECHRNG));
#endif
#ifdef ECKPT
        self->_add_error_code(self, ECKPT, "ECKPT", strerror(ECKPT));
#endif
#ifdef ECLONEME
        self->_add_error_code(self, ECLONEME, "ECLONEME", strerror(ECLONEME));
#endif
#ifdef ECOMM
        self->_add_error_code(self, ECOMM, "ECOMM", strerror(ECOMM));
#endif
#ifdef ECONFIG
        self->_add_error_code(self, ECONFIG, "ECONFIG", strerror(ECONFIG));
#endif
#ifdef ECONNABORTED
        self->_add_error_code(self, ECONNABORTED, "ECONNABORTED", strerror(ECONNABORTED));
#endif
#ifdef ECONNREFUSED
        self->_add_error_code(self, ECONNREFUSED, "ECONNREFUSED", strerror(ECONNREFUSED));
#endif
#ifdef ECONNRESET
        self->_add_error_code(self, ECONNRESET, "ECONNRESET", strerror(ECONNRESET));
#endif
#ifdef ECORRUPT
        self->_add_error_code(self, ECORRUPT, "ECORRUPT", strerror(ECORRUPT));
#endif
#ifdef ECVCERORR
        self->_add_error_code(self, ECVCERORR, "ECVCERORR", strerror(ECVCERORR));
#endif
#ifdef ECVPERORR
        self->_add_error_code(self, ECVPERORR, "ECVPERORR", strerror(ECVPERORR));
#endif
#ifdef EDEADLK
        self->_add_error_code(self, EDEADLK, "EDEADLK", strerror(EDEADLK));
#endif
#if defined(EDEADLOCK) && (!defined(EDEADLK) || EDEADLOCK != EDEADLK)
        self->_add_error_code(self, EDEADLOCK, "EDEADLOCK", strerror(EDEADLOCK));
#endif
#ifdef EDESTADDREQ
        self->_add_error_code(self, EDESTADDREQ, "EDESTADDREQ", strerror(EDESTADDREQ));
#endif
#ifdef EDESTADDRREQ
        self->_add_error_code(self, EDESTADDRREQ, "EDESTADDRREQ", strerror(EDESTADDRREQ));
#endif
#ifdef EDEVERR
        self->_add_error_code(self, EDEVERR, "EDEVERR", strerror(EDEVERR));
#endif
#ifdef EDIRIOCTL
        self->_add_error_code(self, EDIRIOCTL, "EDIRIOCTL", strerror(EDIRIOCTL));
#endif
#ifdef EDIRTY
        self->_add_error_code(self, EDIRTY, "EDIRTY", strerror(EDIRTY));
#endif
#ifdef EDIST
        self->_add_error_code(self, EDIST, "EDIST", strerror(EDIST));
#endif
#ifdef EDOM
        self->_add_error_code(self, EDOM, "EDOM", strerror(EDOM));
#endif
#ifdef EDOOFUS
        self->_add_error_code(self, EDOOFUS, "EDOOFUS", strerror(EDOOFUS));
#endif
#ifdef EDOTDOT
        self->_add_error_code(self, EDOTDOT, "EDOTDOT", strerror(EDOTDOT));
#endif
#ifdef EDQUOT
        self->_add_error_code(self, EDQUOT, "EDQUOT", strerror(EDQUOT));
#endif
#ifdef EDUPFD
        self->_add_error_code(self, EDUPFD, "EDUPFD", strerror(EDUPFD));
#endif
#ifdef EDUPPKG
        self->_add_error_code(self, EDUPPKG, "EDUPPKG", strerror(EDUPPKG));
#endif
#ifdef EEXIST
        self->_add_error_code(self, EEXIST, "EEXIST", strerror(EEXIST));
#endif
#ifdef EFAIL
        self->_add_error_code(self, EFAIL, "EFAIL", strerror(EFAIL));
#endif
#ifdef EFAULT
        self->_add_error_code(self, EFAULT, "EFAULT", strerror(EFAULT));
#endif
#ifdef EFBIG
        self->_add_error_code(self, EFBIG, "EFBIG", strerror(EFBIG));
#endif
#ifdef EFORMAT
        self->_add_error_code(self, EFORMAT, "EFORMAT", strerror(EFORMAT));
#endif
#ifdef EFSCORRUPTED
        self->_add_error_code(self, EFSCORRUPTED, "EFSCORRUPTED", strerror(EFSCORRUPTED));
#endif
#ifdef EFTYPE
        self->_add_error_code(self, EFTYPE, "EFTYPE", strerror(EFTYPE));
#endif
#ifdef EHOSTDOWN
        self->_add_error_code(self, EHOSTDOWN, "EHOSTDOWN", strerror(EHOSTDOWN));
#endif
#ifdef EHOSTUNREACH
        self->_add_error_code(self, EHOSTUNREACH, "EHOSTUNREACH", strerror(EHOSTUNREACH));
#endif
#ifdef EHWPOISON
        self->_add_error_code(self, EHWPOISON, "EHWPOISON", strerror(EHWPOISON));
#endif
#ifdef EIDRM
        self->_add_error_code(self, EIDRM, "EIDRM", strerror(EIDRM));
#endif
#ifdef EILSEQ
        self->_add_error_code(self, EILSEQ, "EILSEQ", strerror(EILSEQ));
#endif
#ifdef EINIT
        self->_add_error_code(self, EINIT, "EINIT", strerror(EINIT));
#endif
#ifdef EINPROG
        self->_add_error_code(self, EINPROG, "EINPROG", strerror(EINPROG));
#endif
#ifdef EINPROGRESS
        self->_add_error_code(self, EINPROGRESS, "EINPROGRESS", strerror(EINPROGRESS));
#endif
#ifdef EINTEGRITY
        self->_add_error_code(self, EINTEGRITY, "EINTEGRITY", strerror(EINTEGRITY));
#endif
#ifdef EINTR
        self->_add_error_code(self, EINTR, "EINTR", strerror(EINTR));
#endif
#ifdef EINVAL
        self->_add_error_code(self, EINVAL, "EINVAL", strerror(EINVAL));
#endif
#ifdef EIO
        self->_add_error_code(self, EIO, "EIO", strerror(EIO));
#endif
#ifdef EIPSEC
        self->_add_error_code(self, EIPSEC, "EIPSEC", strerror(EIPSEC));
#endif
#ifdef EISCONN
        self->_add_error_code(self, EISCONN, "EISCONN", strerror(EISCONN));
#endif
#ifdef EISDIR
        self->_add_error_code(self, EISDIR, "EISDIR", strerror(EISDIR));
#endif
#ifdef EISNAM
        self->_add_error_code(self, EISNAM, "EISNAM", strerror(EISNAM));
#endif
#ifdef EJUSTRETURN
        self->_add_error_code(self, EJUSTRETURN, "EJUSTRETURN", strerror(EJUSTRETURN));
#endif
#ifdef EKEEPLOOKING
        self->_add_error_code(self, EKEEPLOOKING, "EKEEPLOOKING", strerror(EKEEPLOOKING));
#endif
#ifdef EKEYEXPIRED
        self->_add_error_code(self, EKEYEXPIRED, "EKEYEXPIRED", strerror(EKEYEXPIRED));
#endif
#ifdef EKEYREJECTED
        self->_add_error_code(self, EKEYREJECTED, "EKEYREJECTED", strerror(EKEYREJECTED));
#endif
#ifdef EKEYREVOKED
        self->_add_error_code(self, EKEYREVOKED, "EKEYREVOKED", strerror(EKEYREVOKED));
#endif
#ifdef EL2HLT
        self->_add_error_code(self, EL2HLT, "EL2HLT", strerror(EL2HLT));
#endif
#ifdef EL2NSYNC
        self->_add_error_code(self, EL2NSYNC, "EL2NSYNC", strerror(EL2NSYNC));
#endif
#ifdef EL3HLT
        self->_add_error_code(self, EL3HLT, "EL3HLT", strerror(EL3HLT));
#endif
#ifdef EL3RST
        self->_add_error_code(self, EL3RST, "EL3RST", strerror(EL3RST));
#endif
#ifdef ELIBACC
        self->_add_error_code(self, ELIBACC, "ELIBACC", strerror(ELIBACC));
#endif
#ifdef ELIBBAD
        self->_add_error_code(self, ELIBBAD, "ELIBBAD", strerror(ELIBBAD));
#endif
#ifdef ELIBEXEC
        self->_add_error_code(self, ELIBEXEC, "ELIBEXEC", strerror(ELIBEXEC));
#endif
#ifdef ELIBMAX
        self->_add_error_code(self, ELIBMAX, "ELIBMAX", strerror(ELIBMAX));
#endif
#ifdef ELIBSCN
        self->_add_error_code(self, ELIBSCN, "ELIBSCN", strerror(ELIBSCN));
#endif
#ifdef ELNRNG
        self->_add_error_code(self, ELNRNG, "ELNRNG", strerror(ELNRNG));
#endif
#ifdef ELOCKUNMAPPED
        self->_add_error_code(self, ELOCKUNMAPPED, "ELOCKUNMAPPED", strerror(ELOCKUNMAPPED));
#endif
#ifdef ELOOP
        self->_add_error_code(self, ELOOP, "ELOOP", strerror(ELOOP));
#endif
#ifdef EMEDIA
        self->_add_error_code(self, EMEDIA, "EMEDIA", strerror(EMEDIA));
#endif
#ifdef EMEDIUMTYPE
        self->_add_error_code(self, EMEDIUMTYPE, "EMEDIUMTYPE", strerror(EMEDIUMTYPE));
#endif
#ifdef EMFILE
        self->_add_error_code(self, EMFILE, "EMFILE", strerror(EMFILE));
#endif
#ifdef EMLINK
        self->_add_error_code(self, EMLINK, "EMLINK", strerror(EMLINK));
#endif
#ifdef EMOUNTEXIT
        self->_add_error_code(self, EMOUNTEXIT, "EMOUNTEXIT", strerror(EMOUNTEXIT));
#endif
#ifdef EMOVEFD
        self->_add_error_code(self, EMOVEFD, "EMOVEFD", strerror(EMOVEFD));
#endif
#ifdef EMSGSIZE
        self->_add_error_code(self, EMSGSIZE, "EMSGSIZE", strerror(EMSGSIZE));
#endif
#ifdef EMTIMERS
        self->_add_error_code(self, EMTIMERS, "EMTIMERS", strerror(EMTIMERS));
#endif
#ifdef EMULTIHOP
        self->_add_error_code(self, EMULTIHOP, "EMULTIHOP", strerror(EMULTIHOP));
#endif
#ifdef ENAMETOOLONG
        self->_add_error_code(self, ENAMETOOLONG, "ENAMETOOLONG", strerror(ENAMETOOLONG));
#endif
#ifdef ENAVAIL
        self->_add_error_code(self, ENAVAIL, "ENAVAIL", strerror(ENAVAIL));
#endif
#ifdef ENEEDAUTH
        self->_add_error_code(self, ENEEDAUTH, "ENEEDAUTH", strerror(ENEEDAUTH));
#endif
#ifdef ENETDOWN
        self->_add_error_code(self, ENETDOWN, "ENETDOWN", strerror(ENETDOWN));
#endif
#ifdef ENETRESET
        self->_add_error_code(self, ENETRESET, "ENETRESET", strerror(ENETRESET));
#endif
#ifdef ENETUNREACH
        self->_add_error_code(self, ENETUNREACH, "ENETUNREACH", strerror(ENETUNREACH));
#endif
#ifdef ENFILE
        self->_add_error_code(self, ENFILE, "ENFILE", strerror(ENFILE));
#endif
#ifdef ENFSREMOTE
        self->_add_error_code(self, ENFSREMOTE, "ENFSREMOTE", strerror(ENFSREMOTE));
#endif
#ifdef ENOANO
        self->_add_error_code(self, ENOANO, "ENOANO", strerror(ENOANO));
#endif
#ifdef ENOATTR
        self->_add_error_code(self, ENOATTR, "ENOATTR", strerror(ENOATTR));
#endif
#ifdef ENOBUFS
        self->_add_error_code(self, ENOBUFS, "ENOBUFS", strerror(ENOBUFS));
#endif
#ifdef ENOCONNECT
        self->_add_error_code(self, ENOCONNECT, "ENOCONNECT", strerror(ENOCONNECT));
#endif
#ifdef ENOCSI
        self->_add_error_code(self, ENOCSI, "ENOCSI", strerror(ENOCSI));
#endif
#ifdef ENODATA
        self->_add_error_code(self, ENODATA, "ENODATA", strerror(ENODATA));
#endif
#ifdef ENODEV
        self->_add_error_code(self, ENODEV, "ENODEV", strerror(ENODEV));
#endif
#ifdef ENOENT
        self->_add_error_code(self, ENOENT, "ENOENT", strerror(ENOENT));
#endif
#ifdef ENOEXEC
        self->_add_error_code(self, ENOEXEC, "ENOEXEC", strerror(ENOEXEC));
#endif
#ifdef ENOIOCTL
        self->_add_error_code(self, ENOIOCTL, "ENOIOCTL", strerror(ENOIOCTL));
#endif
#ifdef ENOKEY
        self->_add_error_code(self, ENOKEY, "ENOKEY", strerror(ENOKEY));
#endif
#ifdef ENOLCK
        self->_add_error_code(self, ENOLCK, "ENOLCK", strerror(ENOLCK));
#endif
#ifdef ENOLINK
        self->_add_error_code(self, ENOLINK, "ENOLINK", strerror(ENOLINK));
#endif
#ifdef ENOLOAD
        self->_add_error_code(self, ENOLOAD, "ENOLOAD", strerror(ENOLOAD));
#endif
#ifdef ENOMATCH
        self->_add_error_code(self, ENOMATCH, "ENOMATCH", strerror(ENOMATCH));
#endif
#ifdef ENOMEDIUM
        self->_add_error_code(self, ENOMEDIUM, "ENOMEDIUM", strerror(ENOMEDIUM));
#endif
#ifdef ENOMEM
        self->_add_error_code(self, ENOMEM, "ENOMEM", strerror(ENOMEM));
#endif
#ifdef ENOMSG
        self->_add_error_code(self, ENOMSG, "ENOMSG", strerror(ENOMSG));
#endif
#ifdef ENONET
        self->_add_error_code(self, ENONET, "ENONET", strerror(ENONET));
#endif
#ifdef ENOPKG
        self->_add_error_code(self, ENOPKG, "ENOPKG", strerror(ENOPKG));
#endif
#ifdef ENOPOLICY
        self->_add_error_code(self, ENOPOLICY, "ENOPOLICY", strerror(ENOPOLICY));
#endif
#ifdef ENOPROTOOPT
        self->_add_error_code(self, ENOPROTOOPT, "ENOPROTOOPT", strerror(ENOPROTOOPT));
#endif
#ifdef ENOREG
        self->_add_error_code(self, ENOREG, "ENOREG", strerror(ENOREG));
#endif
#ifdef ENOSPC
        self->_add_error_code(self, ENOSPC, "ENOSPC", strerror(ENOSPC));
#endif
#ifdef ENOSR
        self->_add_error_code(self, ENOSR, "ENOSR", strerror(ENOSR));
#endif
#ifdef ENOSTR
        self->_add_error_code(self, ENOSTR, "ENOSTR", strerror(ENOSTR));
#endif
#ifdef ENOSYM
        self->_add_error_code(self, ENOSYM, "ENOSYM", strerror(ENOSYM));
#endif
#ifdef ENOSYS
        self->_add_error_code(self, ENOSYS, "ENOSYS", strerror(ENOSYS));
#endif
#ifdef ENOTACTIVE
        self->_add_error_code(self, ENOTACTIVE, "ENOTACTIVE", strerror(ENOTACTIVE));
#endif
#ifdef ENOTBLK
        self->_add_error_code(self, ENOTBLK, "ENOTBLK", strerror(ENOTBLK));
#endif
#ifdef ENOTCAPABLE
        self->_add_error_code(self, ENOTCAPABLE, "ENOTCAPABLE", strerror(ENOTCAPABLE));
#endif
#ifdef ENOTCONN
        self->_add_error_code(self, ENOTCONN, "ENOTCONN", strerror(ENOTCONN));
#endif
#ifdef ENOTDIR
        self->_add_error_code(self, ENOTDIR, "ENOTDIR", strerror(ENOTDIR));
#endif
#ifdef ENOTEMPTY
        self->_add_error_code(self, ENOTEMPTY, "ENOTEMPTY", strerror(ENOTEMPTY));
#endif
#ifdef ENOTNAM
        self->_add_error_code(self, ENOTNAM, "ENOTNAM", strerror(ENOTNAM));
#endif
#ifdef ENOTREADY
        self->_add_error_code(self, ENOTREADY, "ENOTREADY", strerror(ENOTREADY));
#endif
#ifdef ENOTRECOVERABLE
        self->_add_error_code(self, ENOTRECOVERABLE, "ENOTRECOVERABLE", strerror(ENOTRECOVERABLE));
#endif
#ifdef ENOTRUST
        self->_add_error_code(self, ENOTRUST, "ENOTRUST", strerror(ENOTRUST));
#endif
#ifdef ENOTSOCK
        self->_add_error_code(self, ENOTSOCK, "ENOTSOCK", strerror(ENOTSOCK));
#endif
#ifdef ENOTSUP
        self->_add_error_code(self, ENOTSUP, "ENOTSUP", strerror(ENOTSUP));
#endif
#ifdef ENOTTY
        self->_add_error_code(self, ENOTTY, "ENOTTY", strerror(ENOTTY));
#endif
#ifdef ENOTUNIQ
        self->_add_error_code(self, ENOTUNIQ, "ENOTUNIQ", strerror(ENOTUNIQ));
#endif
#ifdef ENOUNLD
        self->_add_error_code(self, ENOUNLD, "ENOUNLD", strerror(ENOUNLD));
#endif
#ifdef ENOUNREG
        self->_add_error_code(self, ENOUNREG, "ENOUNREG", strerror(ENOUNREG));
#endif
#ifdef ENXIO
        self->_add_error_code(self, ENXIO, "ENXIO", strerror(ENXIO));
#endif
#ifdef EOPCOMPLETE
        self->_add_error_code(self, EOPCOMPLETE, "EOPCOMPLETE", strerror(EOPCOMPLETE));
#endif
#if defined(EOPNOTSUPP) && (!defined(ENOTSUP) || EOPNOTSUPP != ENOTSUP)
        self->_add_error_code(self, EOPNOTSUPP, "EOPNOTSUPP", strerror(EOPNOTSUPP));
#endif
#ifdef EOVERFLOW
        self->_add_error_code(self, EOVERFLOW, "EOVERFLOW", strerror(EOVERFLOW));
#endif
#ifdef EOWNERDEAD
        self->_add_error_code(self, EOWNERDEAD, "EOWNERDEAD", strerror(EOWNERDEAD));
#endif
#ifdef EPASSTHROUGH
        self->_add_error_code(self, EPASSTHROUGH, "EPASSTHROUGH", strerror(EPASSTHROUGH));
#endif
#ifdef EPATHREMOTE
        self->_add_error_code(self, EPATHREMOTE, "EPATHREMOTE", strerror(EPATHREMOTE));
#endif
#ifdef EPERM
        self->_add_error_code(self, EPERM, "EPERM", strerror(EPERM));
#endif
#ifdef EPFNOSUPPORT
        self->_add_error_code(self, EPFNOSUPPORT, "EPFNOSUPPORT", strerror(EPFNOSUPPORT));
#endif
#ifdef EPIPE
        self->_add_error_code(self, EPIPE, "EPIPE", strerror(EPIPE));
#endif
#ifdef EPOWERF
        self->_add_error_code(self, EPOWERF, "EPOWERF", strerror(EPOWERF));
#endif
#ifdef EPROCLIM
        self->_add_error_code(self, EPROCLIM, "EPROCLIM", strerror(EPROCLIM));
#endif
#ifdef EPROCUNAVAIL
        self->_add_error_code(self, EPROCUNAVAIL, "EPROCUNAVAIL", strerror(EPROCUNAVAIL));
#endif
#ifdef EPROGMISMATCH
        self->_add_error_code(self, EPROGMISMATCH, "EPROGMISMATCH", strerror(EPROGMISMATCH));
#endif
#ifdef EPROGUNAVAIL
        self->_add_error_code(self, EPROGUNAVAIL, "EPROGUNAVAIL", strerror(EPROGUNAVAIL));
#endif
#ifdef EPROTO
        self->_add_error_code(self, EPROTO, "EPROTO", strerror(EPROTO));
#endif
#ifdef EPROTONOSUPPORT
        self->_add_error_code(self, EPROTONOSUPPORT, "EPROTONOSUPPORT", strerror(EPROTONOSUPPORT));
#endif
#ifdef EPROTOTYPE
        self->_add_error_code(self, EPROTOTYPE, "EPROTOTYPE", strerror(EPROTOTYPE));
#endif
#ifdef EPWROFF
        self->_add_error_code(self, EPWROFF, "EPWROFF", strerror(EPWROFF));
#endif
#ifdef EQFULL
        self->_add_error_code(self, EQFULL, "EQFULL", strerror(EQFULL));
#endif
#ifdef EQSUSPENDED
        self->_add_error_code(self, EQSUSPENDED, "EQSUSPENDED", strerror(EQSUSPENDED));
#endif
#ifdef ERANGE
        self->_add_error_code(self, ERANGE, "ERANGE", strerror(ERANGE));
#endif
#ifdef ERECYCLE
        self->_add_error_code(self, ERECYCLE, "ERECYCLE", strerror(ERECYCLE));
#endif
#ifdef EREDRIVEOPEN
        self->_add_error_code(self, EREDRIVEOPEN, "EREDRIVEOPEN", strerror(EREDRIVEOPEN));
#endif
#ifdef EREFUSED
        self->_add_error_code(self, EREFUSED, "EREFUSED", strerror(EREFUSED));
#endif
#ifdef ERELOC
        self->_add_error_code(self, ERELOC, "ERELOC", strerror(ERELOC));
#endif
#ifdef ERELOCATED
        self->_add_error_code(self, ERELOCATED, "ERELOCATED", strerror(ERELOCATED));
#endif
#ifdef ERELOOKUP
        self->_add_error_code(self, ERELOOKUP, "ERELOOKUP", strerror(ERELOOKUP));
#endif
#ifdef EREMCHG
        self->_add_error_code(self, EREMCHG, "EREMCHG", strerror(EREMCHG));
#endif
#ifdef EREMDEV
        self->_add_error_code(self, EREMDEV, "EREMDEV", strerror(EREMDEV));
#endif
#ifdef EREMOTE
        self->_add_error_code(self, EREMOTE, "EREMOTE", strerror(EREMOTE));
#endif
#ifdef EREMOTEIO
        self->_add_error_code(self, EREMOTEIO, "EREMOTEIO", strerror(EREMOTEIO));
#endif
#ifdef EREMOTERELEASE
        self->_add_error_code(self, EREMOTERELEASE, "EREMOTERELEASE", strerror(EREMOTERELEASE));
#endif
#ifdef ERESTART
        self->_add_error_code(self, ERESTART, "ERESTART", strerror(ERESTART));
#endif
#ifdef ERFKILL
        self->_add_error_code(self, ERFKILL, "ERFKILL", strerror(ERFKILL));
#endif
#ifdef EROFS
        self->_add_error_code(self, EROFS, "EROFS", strerror(EROFS));
#endif
#ifdef ERPCMISMATCH
        self->_add_error_code(self, ERPCMISMATCH, "ERPCMISMATCH", strerror(ERPCMISMATCH));
#endif
#ifdef ESAD
        self->_add_error_code(self, ESAD, "ESAD", strerror(ESAD));
#endif
#ifdef ESHLIBVERS
        self->_add_error_code(self, ESHLIBVERS, "ESHLIBVERS", strerror(ESHLIBVERS));
#endif
#ifdef ESHUTDOWN
        self->_add_error_code(self, ESHUTDOWN, "ESHUTDOWN", strerror(ESHUTDOWN));
#endif
#ifdef ESOCKTNOSUPPORT
        self->_add_error_code(self, ESOCKTNOSUPPORT, "ESOCKTNOSUPPORT", strerror(ESOCKTNOSUPPORT));
#endif
#ifdef ESOFT
        self->_add_error_code(self, ESOFT, "ESOFT", strerror(ESOFT));
#endif
#ifdef ESPIPE
        self->_add_error_code(self, ESPIPE, "ESPIPE", strerror(ESPIPE));
#endif
#ifdef ESRCH
        self->_add_error_code(self, ESRCH, "ESRCH", strerror(ESRCH));
#endif
#ifdef ESRMNT
        self->_add_error_code(self, ESRMNT, "ESRMNT", strerror(ESRMNT));
#endif
#ifdef ESTALE
        self->_add_error_code(self, ESTALE, "ESTALE", strerror(ESTALE));
#endif
#ifdef ESTART
        self->_add_error_code(self, ESTART, "ESTART", strerror(ESTART));
#endif
#ifdef ESTRPIPE
        self->_add_error_code(self, ESTRPIPE, "ESTRPIPE", strerror(ESTRPIPE));
#endif
#ifdef ESYSERROR
        self->_add_error_code(self, ESYSERROR, "ESYSERROR", strerror(ESYSERROR));
#endif
#ifdef ETIME
        self->_add_error_code(self, ETIME, "ETIME", strerror(ETIME));
#endif
#ifdef ETIMEDOUT
        self->_add_error_code(self, ETIMEDOUT, "ETIMEDOUT", strerror(ETIMEDOUT));
#endif
#ifdef ETOOMANYREFS
        self->_add_error_code(self, ETOOMANYREFS, "ETOOMANYREFS", strerror(ETOOMANYREFS));
#endif
#ifdef ETXTBSY
        self->_add_error_code(self, ETXTBSY, "ETXTBSY", strerror(ETXTBSY));
#endif
#ifdef EUCLEAN
        self->_add_error_code(self, EUCLEAN, "EUCLEAN", strerror(EUCLEAN));
#endif
#ifdef EUNATCH
        self->_add_error_code(self, EUNATCH, "EUNATCH", strerror(EUNATCH));
#endif
#ifdef EUSERS
        self->_add_error_code(self, EUSERS, "EUSERS", strerror(EUSERS));
#endif
#ifdef EVERSION
        self->_add_error_code(self, EVERSION, "EVERSION", strerror(EVERSION));
#endif
#if defined(EWOULDBLOCK) && (!defined(EAGAIN) || EWOULDBLOCK != EAGAIN)
        self->_add_error_code(self, EWOULDBLOCK, "EWOULDBLOCK", strerror(EWOULDBLOCK));
#endif
#ifdef EWRONGFS
        self->_add_error_code(self, EWRONGFS, "EWRONGFS", strerror(EWRONGFS));
#endif
#ifdef EWRPROTECT
        self->_add_error_code(self, EWRPROTECT, "EWRPROTECT", strerror(EWRPROTECT));
#endif
#ifdef EXDEV
        self->_add_error_code(self, EXDEV, "EXDEV", strerror(EXDEV));
#endif
#ifdef EXFULL
        self->_add_error_code(self, EXFULL, "EXFULL", strerror(EXFULL));
#endif

    /* this is ridicules, there has to be a better way. oh,       */
    /* there is, but this has been marked as "depreciated".       */
    /*                                                            */
    /* for (x = 1; x <= sys_nerr; x++) {                          */
    /*                                                            */
    /*     text = strdup(errnoname(x));                           */
    /*     message = strdup(strerror(x));                         */
    /*                                                            */
    /*     self->_add_error_code(self, x, text, message);         */
    /*                                                            */
    /* }                                                          */
    /*                                                            */
    /* thank you standards bodies                                 */
    /*                                                            */

}

