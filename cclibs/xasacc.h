
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 2000 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#ifndef XASACC_H
#define XASACC_H 1

typedef struct {                   /* Used to accumulate print records */
    ACR_IDENT *ident;              /* for forms processing.            */
    ACR_PRINT *print;
} XAS_PRINT_DATA;

typedef struct {
    QUADWORD time_queued;           /* VMS date/time.                   */
    long entry_number;              /* Job entry number.                */
    char *user_name;                /* Username.                        */
    char *queue_name;               /* Output queue.                    */
    char *form_name;                /* Form name.                       */
    char *file_name;                /* Print file.                      */
} XAS_FORMS;

#define XAS_FORM    1               /* XAS Forms Packet.                */
#define XAS1_FIXED  28              /* Fixed size of Forms Packet.      */
#define XAS1_BUFSIZ 227             /* Variable size of Forms Packet.   */

typedef struct {
    BYTE fill[3];                   /* Alignment - value = "XAS".       */
    BYTE type;                      /* XAS user data - XAS_FORM.        */
    long length;                    /* Length of packet.                */
    QUADWORD time_queued;           /* VMS date/time.                   */
    long entry_number;              /* Job entry number.                */
    WORD user_name_offset;          /* Offset to the user name.         */
    WORD queue_name_offset;         /* Offset to the queue name.        */
    WORD form_name_offset;          /* Offset to the form name.         */
    WORD file_name_offset;          /* Offset to the filename.          */
    UBYTE buffer[XAS1_BUFSIZ];      /* Buffer for variable length data. */
} XAS_FORMS_PACKET;                 /* Total size 255 bytes.            */

typedef struct {
    ACR_IDENT *ident;
    XAS_FORMS_PACKET *user;
} XAS_FORMS_DATA;

int  xas_get_packet_type(ACR_USERDATA *data);
int  xas_decode_packet(int type, ACR_USERDATA *user, void *data);
void xas_free_packet(int type, void *data);

#endif

