
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

#ifndef _ERRORS_NCURSES_H_
#define _ERRORS_NCURSES_H_

#include <ncurses/eti.h>

#include "include/error_code.h"

/*-------------------------------------------------------------*/
/* ncurses error code definations                              */
/*-------------------------------------------------------------*/

error_code_t ncurses_codes[] = {
    { E_OK,              "E_OK",              "The routine succeeded" },
    { E_SYSTEM_ERROR,    "E_SYSTEM_ERROR",    "System error occurred" },
    { E_BAD_ARGUMENT,    "E_BAD_ARGUMENT",    "Routine detected an incorrect or out-of-range argument" },
    { E_POSTED,          "E_POSTED",          "Already posted" },
    { E_CONNECTED,       "E_CONNECTED",       "The item is already connected" },
    { E_BAD_STATE,       "E_BAD_STATE",       "Routine was called from an initialization or termination function" },
    { E_NO_ROOM,         "E_NO_ROOM",         "Too large for its window" },
    { E_NOT_POSTED,      "E_NOT_POSTED",      "Has not been posted" },
    { E_UNKNOWN_COMMAND, "E_UNKNOWN_COMMAND", "An unknown request code" },
    { E_NO_MATCH,        "E_NO_MATCH",        "Character failed to match" },
    { E_NOT_SELECTABLE,  "E_NOT_SELECTABLE",  "The designated item cannot be selected" },
    { E_NOT_CONNECTED,   "E_NOT_CONNECTED",   "No items are connected" },
    { E_REQUEST_DENIED,  "E_REQUEST_DENIED",  "Could not process the request" },
    { E_INVALID_FIELD,   "E_INVALID_FIELD",   "Contents of a field are not valid" },
    { E_CURRENT,         "E_CURRENT",         "The field is the current field" },
};

#endif

