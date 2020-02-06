
#ifndef CCLIBS_H
#define CCLIBS_H 1

#include "vms.h"


/*----------------------------------------------------------------------*/
/* Portability functions.                                               */
/*----------------------------------------------------------------------*/

int stricmp(const char *, const char *);
int strnicmp(const char *, const char *, int);
char *strlwr(char *);
char *strupr(char *);
char *strrev(char *);

#define strncmp(x,y) strcmpn(x,y)

/*----------------------------------------------------------------------*/
/* Mailbox functions.                                                   */
/*----------------------------------------------------------------------*/

enum {                    /* Access modes                 */
    mbx_a_read_only = 1,
    mbx_a_write_only,
    mbx_a_both
} mbx_acc_mode;

word  mbx_open(char *, ulong, ulong, ulong, ulong);
int   mbx_read(word, void *, int);
int   mbx_write(word, void *, int);
void  mbx_close(word);
word  mbx_unit(char *);
char *mbx_name(word);
int   mbx_count(word);
void  mbx_eof(word);

/*----------------------------------------------------------------------*/
/* DECnet functions.                                                    */
/*----------------------------------------------------------------------*/

typedef struct {                /* Mailbox buffer for DECnet events.    */
    word type;
    word unit;
    char buffer[512];
} mbxbuf;

typedef struct {
    word mbx_chan;
    word net_chan;
    char *mbxname;
    char *netdev;
    int rbuf_size;
    int action;
    iosb iosb;
    mbxbuf mbx_buf;
    queue alinks;
    queue dlinks;
    int (*accept)();
    int (*resource)();
} dnet_t;

typedef struct {
    word channel;
    word unit;
    iosb iosb;
    unsigned char *client;
    word buff_size;
    void *buffer;
} dnet_link;

/* Server side functions.                                               */

dnet_t *dnet_init(char *, char *, int, int, int *, int *);
int  dnet_server(dnet_t *);
int  dnet_event(dnet_t *, dnet_link *);
void dnet_fini(dnet_t *);

/* Client side functions.                                               */

word dnet_open(char *, char *, char *, char *);
int  dnet_read(word, void *, int);
int  dnet_write(word, void *, int);
void dnet_close(word);

/*----------------------------------------------------------------------*/

#endif

