
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 1996 by Charles A. Measday                                 */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include "get_priv.h"

/*----------------------------------------------------------------------*/

/**/

#define  DIVIDER  '|'          /* Divides fields in database dump file. */

char  *get_field(

#    if __STDC__
        const  char  *s,
        int  *length)
#    else
        s, length)

        char  *s ;
        int  *length ;
#    endif

{
/*
 * Function: get_field.c
 * Version : 1.0
 * Created : 12-Jul-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function get_field() is used to scan a database dump file record.  The
 *    calling program must read the record from the file into a string buffer.
 *    get_field() is then called to retrieve each field in turn from the record.
 *    The following example reads a single record from the database dump file
 *    and displays each field in the record:
 *
 *        char  buffer[128], *s;
 *        int  field_length, length;
 *
 *        ... read a record into BUFFER ...
 *
 *        field_length = 0;
 *        s = get_field(buffer, &field_length);
 *        while (field_length > 0) {
 *            length = str_trim(s, field_length);    -- Trim trailing blanks.
 *            printf("Field = %.*s\n", length, s);
 *            s = get_field(s, &field_length);
 *        }
 *
 *    The function value returned by get_field() is a pointer to the start of the
 *    field in the calling program's string buffer; LENGTH returns the length
 *    of the field. NOTE that get_field() doesn't null-terminate the field - you
 *    need to manipulate the field yourself; e.g., use str_copy(), strncmp(3),
 *    strncpy(3), etc.  Makes you miss FORTRAN-77!
 *
 *    get_field() is intended as a common routine to be used by any programs
 *    that need to input and process a database dump file.  If you change
 *    database programs and the dump file format changes, you'll only need
 *    to change get_field() - the applications program won't need to be changed
 *    (I hope!).
 *
 *    Invocation:
 *
 *        char  *last_field, *next_field, record[N] ;  int  length ;
 *        last_field = record ;  length = 0 ;    -- Initial values.
 *        ...
 *        next_field = get_field(last_field, &length);
 *
 *    where
 *
 *        <last_field>
 *            Points to the start of the last field returned by get_field();
 *            when beginning the scan of a record, set LAST_FIELD to point
 *            to the beginning of the buffer containing the record.
 *
 *        <length>
 *            On input, is the length of the last field returned by 
 *            get_field(); set it to 0 when beginning the scan of a record.  
 *            On output, LENGTH returns the length of the next field, 
 *            excluding the beginning and ending field delimiters (if any).
 *
 *        <next_field>
 *            Returns a pointer to the start of the next field.  The pointer
 *            points into the same buffer that LAST_FIELD points into; the
 *            new field is NOT null-terminated.
 *
 * Modification History
 *
 * Variables Used
 */

    char *bofield;
    char *eofield;

/*
 * Main part of function.
 */

    bofield = (char *)s + *length;        /* Beginning of field. */
    if (*bofield == DIVIDER)  bofield++;

    *length = strlen(bofield);        /* No delimiter at end of string? */

    eofield = strchr(bofield, DIVIDER);    /* End of field. */

    if (eofield != NULL)  *length = (int)(eofield - bofield);

    return(bofield);

}

