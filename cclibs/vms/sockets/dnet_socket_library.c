/*
 * $NCDId: @(#)dnet_socket_library.c,v 4.9 1992/02/12 16:01:34 hardison Exp $
 *
 * Socket library emulation for DECnet.
 * This library was created primarily out of the need for a socket-type 
 * interface for DECnet on a VMS host (originally for the DECnet Font Server 
 * from NCD). Gradually, the various socket calls and lower level functions 
 * evolved into its present form.  The various assumptions and warnings for 
 * each routine are included before its declaration.
 *
 * Another fine product from: Osborne Hardison - NCD - 11/21/91
 *
 * Modifications:
 *
 * - Added support for gethostbyxxxx by querying the NCP database for node 
 *   names and addresses;  subsequent calls to NCPfind use a binary search to 
 *   actually find the given reference (inputs can be in the form of a name, 
 *   a DECnet address, or a hybrid address composed of the last two bytes of 
 *   the active ethernet address).
 *   OBH - 1/5/92
 * - Made changes to accept, bind and nameFromPort for XSCOPE fixes.
 *   OBH - 3/23/92
 *
 */
#pragma module DSL DNET_SOCKET_LIBRARY     /* for easy DEBUG reference */

#include <dvidef.h>
#include <msgdef.h>
#include <syidef.h>
#include <ssdef.h>
#include <stsdef.h>
#include <descrip.h>
#include "nfbdef.h"
#include <iodef.h>
#include <starlet.h>
#include <lib$routines.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include "emulationlib.h"        /* emulation macros and definitions */
#include "portobj.h"            /* port/object translations */
#pragma builtins            /* for _FFS support */

extern volatile int noshare socket_errno;
extern volatile int noshare h_errno;
extern volatile int noshare errno;

static $DESCRIPTOR (netcmd_mbx, "NETCMD_MBX");
static $DESCRIPTOR (net_device, "_NET:");
static unsigned short lisChannel = 0;
static unsigned short mbxChannel = 0;
static unsigned short netChannel = 0;
static int stackIndex;
static int timerExpired;
static int zero = 0;
static int info_cnt;
static int NCPtot = 0;
static int mbx_created = 0;
static int initChanTable = 1;
static int mbxMsgLen = MAX_MSG;
static char *ncb = NULL;
static struct io_stat_blk netiosb;
static NCPstruct *pNCP = NULL;
static struct chantable *ChanTableInfo[FD_SETSIZE];
static struct stackstruct stack[MAX_LINKS];

fd_set channelfds;
fd_set *allChannels = &channelfds;

#define Number(a)       (sizeof(a)/sizeof(a[0]))


/* 
 * SUPPORT ROUTINE DECLARATIONS
 * Routines to support socket library emulation routines
 */
void         clearChanTable ();    /* Sets up and clears Channel Table */
int         connectNet ();        /* Connects to NET: and network MBX */
void         *dnMalloc ();        /* Special malloc */
int         dnFree ();        /* Special free   */
char         *initNCB ();        /* Net control block init routine */
int         isEtherAddr ();        /* funct to determine is passed str is enet addr */
int         isDnetAddr ();        /* funct to determine is passed str is dnet addr */
int         issueNetCmdRead ();    /* funct to issue a network read w/AST */
int         issueRead ();        /* funct to issue a socket read w/AST */
void         linkInit ();        /* link initilization routine */
void         linkTerm ();        /* link termination routine */
struct dsc$descriptor *nameFromPort ();    /* funct to return an object from a port designation */
static int     NCPpredicate ();    /* funct to pass to qsort */
int         NCPinit ();        /* sets up the DECnet node data */
int         NCPfind ();        /* finds DECnet node info from name or addr */
void         netCmdAst ();        /* AST for network messages */
void         notUsed ();        /* funct for unrecognized network messages */
int         processNetCmd ();    /* processes network messages */
int        readAst ();        /* AST for all asynch select socket reads */
void         timeoutAst ();        /* funct for select timeouts */
void         upper ();        /* lower to upper conversion */


/* 
 * EMULATION ROUTINE DECLARATIONS
 * Routines for the actual socket library emulation
 */
unsigned short     accept ();
int         bcmp ();
void         *bcopy ();
int         bind ();
int         bzero ();
int         connect ();
int         ffs ();
int         getdtablesize ();
struct hostent     *gethostbyaddr ();
struct hostent     *gethostbyname ();
int         gethostname ();
int         getpeername ();
int         gettimeofday ();
unsigned short     htons ();
int         htonl ();
int         inet_addr ();
int         listen ();
int         select ();
int         setsockopt ();
unsigned short     socket ();
int         socket_close ();
int         socket_ioctl ();
void         socket_perror ();
int         socket_read ();
int         socket_write ();
int         socket_writev ();


/*
 * SUPPORT ROUTINES
 */

/*
 * clearChanTable():
 * Module to allocate a chantable-struct-sized chunk of VM (if the entry in
 * the channel table is NULL for the passed shifted channel) or to free the 
 * current chunk of VM that is used to maps the passed channel's information 
 * (as in the case of a link termination).
 */
void
clearChanTable (shiftChan)
    unsigned short shiftChan;
{
    int    i;
    static int movlen = sizeof(struct chantable);

    if (initChanTable)
    {
        initChanTable = 0;
        for (i = 0; i < FD_SETSIZE; i++)
            ChanTableInfo[i] = (struct chantable *) NULL;
    }
/* 
 * Allocate or free the channel table info area in VM depending on who called 
 * this procedure.
 */
    if (ChanTableInfo[shiftChan])
    {
        if (ChanTableInfo[shiftChan]->nodename)
            DNETFREE (ChanTableInfo[shiftChan]->nodename);
        if (ChanTableInfo[shiftChan]->base)
            DNETFREE (ChanTableInfo[shiftChan]->base);
        if (ChanTableInfo[shiftChan]->backLogQueue)
            DNETFREE (ChanTableInfo[shiftChan]->backLogQueue);
        DNETFREE (ChanTableInfo[shiftChan]);
        ChanTableInfo[shiftChan] = (struct chantable *) NULL;
        FD_CLR((shiftChan<<4), allChannels);
        return;
    }
    ChanTableInfo[shiftChan] = (struct chantable *) DNETALLOC (movlen);
    LIB$MOVC5 (&zero, NULL, "\0", &movlen, (char *) ChanTableInfo[shiftChan]);
        ChanTableInfo[shiftChan]->base = DNETALLOC(MAX_QIO);
        ChanTableInfo[shiftChan]->delta = ChanTableInfo[shiftChan]->base;
}


/*
 * connectNet
 * Creates a link to the _NET: device and, if no MBX link  exists, creates one
 * to the network mailbox.
 */
int
connectNet ()
{
        int    status;

        if (!mbx_created)
        {
            status = SYS$CREMBX (TEMP_MBX, &mbxChannel, MAX_MSG, BUF_QUO, 0, 0, &netcmd_mbx);
        if (VMSFAIL(status)) LIB$SIGNAL(status);
            mbx_created++;
        }
    status = SYS$ASSIGN (&net_device, &netChannel, 0, &netcmd_mbx);
    if (VMSFAIL(status)) LIB$SIGNAL(status);
#ifdef DNDEBUG
        fprintf(stderr, "connectNet: netChannel: %d\n", netChannel);
#endif
        return status;
}


/*
 * malloc and free routines:
 * These are specifically tied to LIB$GET_VM and LIB$FREE_VM to
 * 1) speed things up a notch, and
 * 2) get away from using buggy CRTL malloc.
 */
void *
dnMalloc(size)
        int size;
{
        int status;
        int soql = sizeof(long) * 8;
        char *addr;

        size = (((((size + sizeof(int)) + (soql - 1))/soql) +1) * soql);
        status = LIB$GET_VM(&size, (unsigned long *) &addr);
        if (VMSFAIL(status)) LIB$SIGNAL(status);
        bcopy ((char *) &size, addr, sizeof(int));
        addr += sizeof(int);
        return addr;
}


int
dnFree(addr)
        char *addr;
{
        int status;
        unsigned int size;
            
        if (!addr) return 1;
        addr -= sizeof(int);
        bcopy (addr, (char *) &size, sizeof(int));
        status = LIB$FREE_VM(&size, (unsigned long *) &addr);
        if (VMSFAIL(status)) LIB$SIGNAL(status);
        return 1;
}


/* 
 * initNCB
 * Module to create Network Control Block as in, 
 * { "MV3100::",'"',"TASK=FONT$FS/",0,'"' } from something that looks 
 * like this: "decnet/mv3100::FONT$FS".  Note that the object name is 
 * assumed to be at the pointer located one word off of the "name" pointer 
 * (name+2), where name points to the sockaddr structure from the program 
 * that called (connect()).
 */
char *
initNCB (name, objlen, task, node)
    struct sockaddr *name;
    unsigned short    *objlen;
    char *task;
    char *node;
{
    char *pncb = (char *) DNETALLOC (NCBSIZE);
    char *op;
    char *onp;

    if (!node)
    {
        {
            char *oop = (char *) ((char *) name + sizeof(name->sa_family));
            char *t = &op;
            bcopy (oop, t, sizeof(&op));
        }
            if (!isalpha(*op)) return NULL;
        upper(op);
        if (!(memcmp(op, DNET_PREFIX, strlen(DNET_PREFIX)))) 
            op += strlen(DNET_PREFIX);
        else
                return NULL;
    }
    else
        op = node;
    bcopy(op, (char *) pncb, strlen(op));
    onp = strchr(pncb, ':') + 2;
    if (!node) op = strchr(op, ':') + 2;
    *onp++ = '"';
    strcpy(onp, task);
    onp += strlen(task);
    if (!node)
    {
        strcpy(onp, op);
        onp += strlen(op);
        *onp++ = '/';
    }
    *onp++ = '\0';
    *onp++ = '\0';
    *onp++ = '"';
    *objlen = onp - (char *) pncb;
    return pncb;
}


/*
 * isEtherAddr
 * Boolean routine to determine if a passed client node name
 * is a valid DECnet name or a ethernet ad hoc name derived from the
 * last two bytes of the active ethernet address.
 */
int
isEtherAddr (caddr)
        char *caddr;
{
        char *a;

        if ((!caddr) || (!*caddr)) return 0;
        if (strchr(caddr, '.')) return 0;
        for ( a = caddr; *a; a++)
            if((*a < '0') || (*a > '9')) return 0;
        if (a == caddr) return 0;
#ifdef DNDEBUG
        fprintf(stderr, "isEtherAddr: %s is an ethernet address\n",caddr);
#endif
        return 1;
            
}


/*
 * isDnetAddr
 * Boolean routine to determine if a passed client node name
 * is a valid DECnet address.
 */
int
isDnetAddr (caddr)
        char *caddr;
{
        if (strchr(caddr, '.')) return 1;
        return 0;
}


/*
 * issueNetCmdRead
 * Reading from the Network Command Mailbox willbe done on the mbxChannel, but
 * will be treated as a read in the NET: channel, since it is the channel that the
 * upper layer sees.
 */
int
issueNetCmdRead (chan)
    unsigned short chan;
{
        int status;

        if (!ChanTableInfo[(mbxChannel>>4)]->readAstPend)
        {
#ifdef DNDEBUG
            fprintf(stderr, "issueNetCmdRead: read pending on channel %d from chan %d\n", mbxChannel, chan);
#endif
            ChanTableInfo[(mbxChannel>>4)]->readAstPend = 1;
            status = SYS$QIO (0, mbxChannel, IO$_READVBLK, &netiosb, &netCmdAst, NET_CMD,
                                &nb, mbxMsgLen, 0, 0, 0, 0);
            if (VMSFAIL(status)) LIB$SIGNAL(status);
        }
        return status;
}


/*
 * issueRead (chan)
 * Routine to setup a read AST on the passed channel.  In our emulation
 * we must check the channel's buffers to see if any remaining data is left
 * to be read.  In the case that there is some remainder, the routine will 
 * "fake" an AST event by shoving interrupt information in the stack.  Otherwise
 * the routine sets up the AST normally with the buffer being the base of the 
 * vm area setup.
 */
int
issueRead (chan)
    unsigned short chan;
{
    astParam ap;
    int shiftChan = chan>>4;
    char *vmbase = ChanTableInfo[shiftChan]->base;
    char *vmdelta = ChanTableInfo[shiftChan]->delta;
    int vmlen = ChanTableInfo[shiftChan]->len;
    int remainder = (vmbase + vmlen) - vmdelta;
        int status;
/* 
 * Insert a fake result in the stack if there is unread data in our channel's buffer. 
 * Then return without setting up the read attention AST QIO.
 */
    if (remainder > 0)
    {
        stack[stackIndex].channel = chan;
        stack[stackIndex].type = NET_RD;
        stackIndex++;
        ChanTableInfo[(chan>>4)]->readAstPend = 0;
        ChanTableInfo[(chan>>4)]->readAstDone = 1;
#ifdef DNDEBUG
        fprintf(stderr, "issueRead: (puesdoread) to channel %d remainder %d\n", chan, remainder);
#endif
        return SS$_NORMAL;
    }
        ChanTableInfo[shiftChan]->base = vmbase;
        ChanTableInfo[shiftChan]->delta = vmdelta = vmbase;
        ChanTableInfo[shiftChan]->len = vmlen = 0;

    ap.ast.type = NET_RD;
    ap.ast.chan = chan;

#ifdef DNDEBUG
        fprintf(stderr, "issueRead: read pending on channel %d\n", chan);
#endif
    ChanTableInfo[shiftChan]->readAstDone = 0;
    ChanTableInfo[shiftChan]->readAstPend = 1;
        status = SYS$QIO (0, chan, IO$_READVBLK, &ChanTableInfo[shiftChan]->read_iosb, 
            &readAst, ap.astid, vmbase, MAX_QIO, 0, 0, 0, 0);
        if (VMSFAIL(status)) LIB$SIGNAL(status);
}


/*
 * linkInit
 * Routine to intercept the network message and parse it into a node name.
 */
void
linkInit ()
{
    int i = nb.mbxMsg[0] + 1;
    int start = i + 1;

    info_cnt = nb.mbxMsg[i];
    ncb = &nb.mbxMsg[start];
}


/*
 * linkTerm
 * Routine to terminate a link (low level - socket_close is high level) given
 * a channel spec or the network message block's unit number.
 */
void
linkTerm(sChan)
    unsigned short sChan;
{
    int status;
    unsigned short channel;
    unsigned short next;
    int currChannel;
    int i;

        next = 0;
    if (sChan)
        {
            if (!ChanTableInfo[sChan]) return;
            if (((sChan<<4) == netChannel) && (lisChannel)) netChannel = lisChannel;
            if (ChanTableInfo[sChan]->chanDassgn) return;
        next = sChan;
        goto bash;
    }
    for (i = 0; i < (FD_SETSIZE/NFDBITS); i++)
    {
        currChannel = (int) allChannels->bits[i];
        while (currChannel)
        {
            next = (ffs(currChannel) - 1) + (i * NFDBITS);
            if (ChanTableInfo[next]->unit == nb.unit) goto bash;
            currChannel &= ~(1 << next);
        }
    }
bash:
    if ((!next) || (next > FD_SETSIZE))
            return;
    else
    {
        channel = next<<4;
#ifdef DNDEBUG
        fprintf(stderr, "linkTerm: terminating link to %d\n",channel);
#endif
        status = SYS$DASSGN (channel);
        if (VMSFAIL(status)) LIB$SIGNAL(status);
            ChanTableInfo[next]->chanDassgn = 1;
    }
}


/*
 * nameFromPort
 * A hack to translate the passed port number to a DECnet OBJECT name. Its ugly
 * but hey, who asked you?
 */
struct dsc$descriptor
*nameFromPort (sa)
    struct sockaddr *sa;
{
    union
    {
        unsigned short port;
        char t[2];
    } temp;
        int i = 0;

    temp.t[0] = sa->sa_data[1];
    temp.t[1] = sa->sa_data[0];
        for (i = 0; i < Number(transTable); i++)
        {
            if (transTable[i].port == temp.port)
            {
                if (transTable[i].port = X11_PORT)
                x11_dsc.dsc$a_pointer[3] = 0x30;
                return transTable[i].object;
            }
        }
/*
 * Of course - the obligatory special case: used when someone tries to 
 * access a X11 "reserved" port 6001-9 (beyond 6009, I don't care!)
 */
    if ((temp.port > X11_PORT) && (temp.port < X11_PORT + 10))
    {
        x11_dsc.dsc$a_pointer[3] = ((temp.port - X11_PORT) + 0x30);
        return &x11_dsc;
    } 
    fprintf (stderr, "nameFromPort: unrecognized port request: %d\n");
    return NULL;
}


/*
 * NCPpredicate
 * Routine that is passed to qsort.
 */
static int
NCPpredicate (c1, c2)
        NCPstruct *c1;
        NCPstruct *c2;
{
        return (memcmp (c1->nodebuf, c2->nodebuf, MAX(strlen(c1->nodebuf), strlen(c2->nodebuf))));
}


/*
 * NCPinit()
 * Routine to load the NCP table of node/addresses from its data base using 
 * the Network File Block (NFB$) defined constants for node and address 
 * matching of the on-line data base.
 */
int
NCPinit()
{
        int    i;
        int    j;
        int    add;
        int    retLen = 0;
        int    status;
        int    seg = MAXDNETNODES + 1;
    unsigned short netNCPChan;
        struct io_stat_blk iosb;
        char     *delta;
        struct nfbdef nfb = { NFB$C_FC_SHOW, 0, NFB$C_DB_NDI, NFB$C_OP_EQL,
                      NFB$C_WILDCARD, NFB$C_WILDCARD, 0, 0, 0,
                      NFB$C_NDI_NNA, NFB$C_NDI_TAD, NFB$C_ENDOFLIST };
        struct dsc$descriptor nfbDes = {sizeof(nfb), 0, 0, &nfb};
        char *keyArea = DNETALLOC(NFB$C_CTX_SIZE);
        struct dsc$descriptor kaDes = {NFB$C_CTX_SIZE, 0, 0, keyArea};
        char *bufArea = DNETALLOC(BUF_QUO);
        struct dsc$descriptor bufDes = {BUF_QUO, 0, 0, bufArea};
        struct nfbdef1 *prm;
        struct nfbdef2 *val;

        i = 0;
/*
 * Assign a temporary channel to _NET: to return NCP db info
 */
    status = SYS$ASSIGN (&net_device, &netNCPChan, 0, 0);
    if (VMSFAIL(status)) LIB$SIGNAL(status);
        LIB$MOVC5 (&zero, NULL, "\0", &kaDes, keyArea);
        status = SYS$QIOW (0, netChannel, IO$_ACPCONTROL, &iosb, 0, 0, 
            &nfbDes, &kaDes, &retLen, &bufDes, 0, 0);
        if (!VMSFAIL(status)) status = iosb.status;
        if ((VMSFAIL(status)) && (status != SS$_ENDOFFILE)) LIB$SIGNAL(status);
/*
 * Main loop for calling ACPCONTROL on the _NET: device, returning database information.
 */
        while (status != SS$_ENDOFFILE)
        {
            delta = bufArea;
            if (seg > MAXDNETNODES)        /* add memory in puesdo realloc mode */
            {
                NCPstruct *tmp = (NCPstruct *) 
                    DNETALLOC((i+MAXDNETNODES)*sizeof(NCPstruct));
                if(!tmp) exit(SS$_INSFMEM);
                if (pNCP)
                {
                   bcopy ((char *) pNCP, (char *) tmp, sizeof(NCPstruct) * i);
                   DNETFREE(pNCP);
                }
               pNCP = tmp;
                seg = 0;
            }
/*
 * Loop through the NFB records that the NETACP passed back to this routine.
 */
            for (j = 0; nfb.fldid[j] != NFB$C_ENDOFLIST; ++j)
            {
                add = 0;
                prm = (struct nfbdef1 *) &nfb.fldid[j];
                val = (struct nfbdef2 *) delta;
                switch (prm->overlay.fields.typ)
                {
                case NFB$C_TYP_BIT:
                case NFB$C_TYP_LNG:
                pNCP[i].area = (short) (val->overlay.lngValue / 1024);
                pNCP[i].node = (short) (val->overlay.lngValue % 1024);
                    add = sizeof(long);
                    break;
                case NFB$C_TYP_STR:
                        pNCP[i].nodebuf = DNETALLOC (NODNAMSIZ);
                    bcopy (&val->overlay.string.strText, pNCP[i].nodebuf,
                        val->overlay.string.strCount);
                    pNCP[i].nodebuf[val->overlay.string.strCount] = 0x0;
                    add = val->overlay.string.strCount + sizeof(short);
                    break;
                default:
                    fprintf(stderr, "NCPinit: unrecognized %d\n",val->overlay.lngValue);
                    add = sizeof(long);
                }
                delta += add;
            }
#ifdef NCPDEBUG
               fprintf(stderr, "NCPinit: %d.%d: %s\n",pNCP[i].area, pNCP[i].node, pNCP[i].nodebuf);
#endif /* NCPDEBUG */
            i++;
            seg++;
            status = SYS$QIOW (0, netNCPChan, IO$_ACPCONTROL, &iosb, 0, 0, 
                &nfbDes, &kaDes, &retLen, &bufDes, 0, 0);
            if (!VMSFAIL(status)) status = iosb.status;
            if ((VMSFAIL(status)) && (status != SS$_ENDOFFILE)) LIB$SIGNAL(status);
    }
/*
 * Hopefully we have some nodes here to sort...
 */
        if (i)
        {
            NCPtot = i;
            qsort ((char *) pNCP, NCPtot, sizeof(NCPstruct), NCPpredicate);
            SYS$DASSGN (netNCPChan);
#ifdef NCPDEBUG
            fprintf(stderr, "NCPinit: %d nodes found\n", NCPtot);
#endif
        }
        else
            fprintf(stderr, "NCPinit: no nodes found!\n");
}


/*
 * NCPfind
 * Ah, the luscious jewel of the Nile, itself!  This baby looks up a given 
 * node id in the NCP table (built in NCPinit()).  The id can be in one of 
 * three forms:
 * 1) DECnet Node Name - Phase IV has a max of 6 characters
 * 2) DECnet Node Address - up to the max of 63.1023
 * 3) Unknown Node Address - this sucker comes from the active ethernet address.
 *    It is composed of all numbers (illegal for a node name spec). The rtn can
 *    translate this address to become a DECnet Node Address type above. The 
 *    fact that a node comes in as a hybrid active ethernet address name 
 *    portends that the node is not known by NCP.
 * The routine returns a binary value of the address in the sockaddr passed 
 * area and the resolved node name/address in "client".
 */
int
NCPfind (sChan, psock, client)
        unsigned short sChan;
        char    *psock;
        char     *client;
{
        int     status;
        int     i;
        short     node;
        short    area;
        int    result;
        int    last;
        int    strt;
        char     *b1;
        char     *b2;
        char     *aoff;
        static NCPstruct dum = {NULL, 0, 0};
        NCPstruct *pNCPf;
        struct sht
        {
            union
            {
                struct chs
                {
                    char c1;
                    char c2;
                }c;
                short s1;
            }o;
        }s;

        if (!pNCP) NCPinit();
        if (sChan)
        {
            aoff = (char *) (psock + sizeof(long));
            if (ChanTableInfo[sChan])
            {
                if (ChanTableInfo[sChan]->node + ChanTableInfo[sChan]->area)
                {
                    if (ChanTableInfo[sChan]->nodename)
                    {
                            bcopy (ChanTableInfo[sChan]->nodename, client, 
                            strlen(ChanTableInfo[sChan]->nodename)+1);
                        client[strlen(ChanTableInfo[sChan]->nodename)] = 0x0;
                    }
                    else
                        sprintf (client, "%d.%d", ChanTableInfo[sChan]->area, 
                            ChanTableInfo[i]->node);
                    bcopy ((char *) &ChanTableInfo[sChan]->area, aoff, sizeof(short));
                    bcopy ((char *) &ChanTableInfo[sChan]->node, 
                        (char *) (aoff + sizeof(short)), sizeof(short));
                    return 0;
                    }
            }
        }
        else
            aoff = psock;
        if ((!client) || (!*client)) fprintf(stderr, "NCPaddr: error no client spec in call\n");
    upper(client);
        if (strchr(client, ':')) *((char *) strchr(client, ':')) = '\0';
        if (isEtherAddr(client))     /* it's an active ethernet hybrid address */
        {
            result = atoi(client);
            DNETFREE(client);
            client = DNETALLOC(NODNAMSIZ);
            sprintf(client, "%d.%d", result/1024, result % 1024);
        }
        if (isDnetAddr(client))     /* it's a DECnet address (eg., "1.22") */
        {
            b1 = client;
            b2 = strchr (client, '.');
            *b2++ = 0x0;
            area = (short) atoi(b1);
            node = (short) atoi(b2);
        sprintf (client, "%d.%d", area, node);
            for (i = 0; i < NCPtot; i++)
            {
                if ((pNCP[i].area == area) && (pNCP[i].node == node)) break;
            }
        }
        else                 /* it's a node name (eg., "NCD999") */
        {
            dum.nodebuf = client;
            i = -1;
            if((pNCPf = (NCPstruct *) bsearch (&dum, pNCP, NCPtot, 
                sizeof(NCPstruct), NCPpredicate)))
                i = (pNCPf - pNCP);
        }
/*
 * We should now have either a valid index into the NCP table or a 
 * indication that the node name/address is not currently known by this 
 * host node.
 */
    if ((i > -1) && (i < NCPtot))
        {
            if (pNCP[i].nodebuf)
                bcopy (pNCP[i].nodebuf, client, strlen(pNCP[i].nodebuf)+1);
            else
                sprintf (client, "%d.%d", pNCP[i].area, pNCP[i].node);
            bcopy ((char *) &pNCP[i].area, aoff, sizeof(short));
            bcopy ((char *) &pNCP[i].node, (char *) (aoff + sizeof(short)), sizeof(short));
            if (sChan && ChanTableInfo[sChan])
            {
                if (!ChanTableInfo[sChan]->nodename) 
                    ChanTableInfo[sChan]->nodename = DNETALLOC(NODNAMSIZ);
                bcopy (client, ChanTableInfo[sChan]->nodename, strlen(client) + 1);
                ChanTableInfo[sChan]->nodename[strlen(client)] = 0x0;
                ChanTableInfo[sChan]->node = pNCP[i].node;
                ChanTableInfo[sChan]->area = pNCP[i].area;
            }
            return 0;
        }
        else
        {
            fprintf(stderr, "NCPfind: %s not found in node lookup\n",client);
            return -1;
        }
}


/*
 * netCmdAst
 * AST routine the fires for incoming network messages.
 */
void
netCmdAst (type)
    int type;
{
    int i;
    int start;

        ChanTableInfo[(mbxChannel>>4)]->readAstPend = 0;
    stack[stackIndex].channel = netChannel;
    stack[stackIndex].type = NET_CMD;
    stackIndex++;
    WAKE_UP_DAMMIT;
}


void
notUsed()
{
    fprintf (stderr, "Unused message %d detected\n", nb.netCmd);
}


/*
 * processNetCmd
 * Routine to process a network message.
 */
int
processNetCmd (pChan)
        unsigned short *pChan;
{
        unsigned short sChan = ((*pChan)>>4);
/*
 * Note that the current netChannel could have disappeared during the interim (as
 * with an xscope connect()ed link with the X11 server), so that issuing the netcmd read
 * using that channel would be invalid on the next network directive.  Therefore, there
 * must be a provision for testing the passed channel information.
 */
        if (!ChanTableInfo[sChan])
        {
            if (lisChannel) 
                *pChan = lisChannel;
            else
                fprintf(stderr, "processNetCmd: error on invalid netChannel %d\n", *pChan);
        }
/* Reissue on the passed or updated channel */
    issueNetCmdRead (*pChan);
#ifdef DNDEBUG
        fprintf(stderr, "processNetCmd: netiosb: %d status: %d\n", netiosb.msg_len, netiosb.status);
#endif

    switch (nb.netCmd)
    {
        case MSG$_CONNECT    :    
#ifdef DNDEBUG
                        fprintf(stderr, "Link init\n");
#endif
                        linkInit (); return 1; break;
                case MSG$_ABORT      :
                case MSG$_DISCON     :
                case MSG$_EXIT       :
                case MSG$_PATHLOST   :
                case MSG$_PROTOCOL   :
                case MSG$_THIRDPARTY :
                case MSG$_TIMEOUT    :    
#ifdef DNDEBUG
                        fprintf(stderr, "processNetCmd: linkTerm\n");
#endif
                    linkTerm ((unsigned short) 0); break;
                case MSG$_NETSHUT    :    fprintf (stderr, "DECnet shutdown detected - exiting\n"); 
                    exit(1);
                case MSG$_CONFIRM    :  
#ifdef DNDEBUG
                        fprintf(stderr, "Connection confirmed\n"); 
#endif
                        break;
                case MSG$_INTMSG     :  break;
                case MSG$_REJECT     :  break;
                default              :    notUsed(); break;
    }
    return 0;
}


/* readAst (ap)
 * AST routine to update the stack, increment the channel's length variable,
 * and wake up the server.
 */
int
readAst (ap)
    struct astblk ap;
{
        unsigned short sChan = ap.chan>>4;

        if (ChanTableInfo[sChan])
        {
            stack[stackIndex].channel = ap.chan;
            stack[stackIndex].type = ap.type;
        stackIndex++;
        ChanTableInfo[sChan]->len = ChanTableInfo[sChan]->read_iosb.msg_len;
        ChanTableInfo[sChan]->readAstPend = 0;
        ChanTableInfo[sChan]->readAstDone = 1;
            WAKE_UP_DAMMIT;
        }

	return(0);

}


/*
 * timeoutAst
 * Routine that fires after the select() timer expires.
 */
void
timeoutAst()
{
    timerExpired = 1;
    WAKE_UP_DAMMIT;
}


/*
 * upper
 * Routine to convert a NUL terminated string to uppercase.
 */
void
upper (s)
        char    *s;
{
        int     i;
        char    *tp;

        for (i = 0, tp = s; i < strlen(s); i++, tp++)
                if (islower(*tp)) *tp = toupper(*tp);
}


/*
 * EMULATION ROUTINES
 */


/*
 * accept():
 * Module to emulate the "acceptance" of a connection over a "listen()"ing 
 * channel. In the current paradigm, only one listening channel is supported 
 * (called "lisChannel"); even though the actual network command(s) arrive 
 * over the channel assigned to the network command mailbox (w/ the NET: 
 * device; called "mbxChannel").  The "chan" being passed to this and all 
 * other socket emulation routines dealing with network issues (i.e.,
 * the only non-client channel) is the listening channel established in 
 * socket().  All references to the Net Command Mailbox channel are internal 
 * to this library, and are opaque to the upper layer program.
 */
unsigned short
accept (chan, psock, psock_l)
    unsigned short chan;
    struct sockaddr *psock;
    long psock_l;
{
        int status;
    int dvi_unit;
    unsigned short channel;
    unsigned short sChan;
    struct io_stat_blk iosb;
    struct dsc$descriptor_s ncbd;
    struct 
    {
        short buf_len;
        short code;
        int *ret_code;
        int ret_len;
        int term;
    } dviitm = { 4, DVI$_UNIT, &dvi_unit, 0, 0};

#ifdef DNDEBUG
        fprintf(stderr, "accept: start - passed chan %d\n", chan);
#endif /* DNDEBUG */
    if (!ncb) 
    {
        fprintf (stderr, "accept: no ncb received!\n");
        return -1;
    }
    ncb[info_cnt] = '\0';
        ncbd.dsc$w_length = info_cnt;
        ncbd.dsc$a_pointer = ncb;
        ncbd.dsc$b_dtype = DSC$K_DTYPE_T;
        ncbd.dsc$b_class = DSC$K_CLASS_S;

        if (!mbx_created)
        {
            fprintf(stderr, "accept: error - mailbox not created yet\n");
            exit();
        }
        status = SYS$ASSIGN (&net_device, &channel, 0, &netcmd_mbx);
    if (VMSFAIL(status)) LIB$SIGNAL(status);
    status = SYS$GETDVI (0, channel, 0, &dviitm, &iosb, 0, 0, 0);
    if (VMSFAIL(status)) LIB$SIGNAL(status);
        sChan = channel>>4;
    clearChanTable ((sChan));
    ChanTableInfo[(sChan)]->unit = dvi_unit;
    ChanTableInfo[(sChan)]->nodename = DNETALLOC (NODNAMSIZ);
    bcopy ((char *) ncb, ChanTableInfo[(sChan)]->nodename, info_cnt);
        ChanTableInfo[(sChan)]->user = (strchr(ChanTableInfo[(sChan)]->nodename, '=') + 1);
        *((char *) strchr(ChanTableInfo[(sChan)]->nodename,':') + 2) = '\0';
        *((char *) strchr(ChanTableInfo[(sChan)]->user,'/')) = '\0';
    FD_SET(channel, allChannels);
#ifdef DNDEBUG
    fprintf(stderr, "accept: from Node: %s User: %s on channel %d\n",
            ChanTableInfo[(sChan)]->nodename, ChanTableInfo[(sChan)]->user, channel);
#endif
        status = SYS$QIOW (0, channel, IO$_ACCESS, &iosb, 0, 0, 0, &ncbd, 0, 0, 0, 0);
    if(!VMSFAIL(status) && !VMSFAIL(iosb.status))
    {
        ncb = NULL;
            return channel;
    }
    if (VMSFAIL(status)) LIB$SIGNAL (status);
    if (VMSFAIL(iosb.status)) LIB$SIGNAL(iosb.status);
    return -1;
}


/*
 * B routines: bcmp, bcopy, bzero
 * These routines are declared here in function form to provide
 * a global linker resolution for references in upper level code.
 */
int
bcmp (s1, s2, l)
    char *s1;
    char *s2;
    int l;
{
    return (BCMP(s1, s2, l));
}


void *
bcopy (s, d, l)
    char *s;
    char *d;
    int l;
{
    return BCOPY(s, d, l);
}


/*
 * bind():
 * bind the NET channel to the correct object name.
 */
int
bind (chan, sa, sa_len)
    unsigned short chan;
    struct sockaddr *sa;
    unsigned int sa_len;
{
    int status;
    struct io_stat_blk iosb;
    struct dsc$descriptor_s nfbd;
    struct dsc$descriptor_s *object_name;

#ifdef DNDEBUG
        fprintf(stderr, "bind: start - passed chan %d\n", chan);
#endif /* DNDEBUG */
        nfbd.dsc$w_length = sizeof(nfb);
        nfbd.dsc$a_pointer = &nfb;
        nfbd.dsc$b_dtype = DSC$K_DTYPE_T;
        nfbd.dsc$b_class = DSC$K_CLASS_S;
/*
 * Bind to the declare channel in the ChanTableInfo struct.
 */
    object_name = nameFromPort (sa);
    status = SYS$QIOW (0, chan, IO$_ACPCONTROL, &iosb, 0, 0, &nfbd, object_name, 0, 0, 0, 0);
    if (VMSFAIL(status) || VMSFAIL(iosb.status)) return -1;
#ifdef DNDEBUG
        fprintf(stderr, "bind: on channel %d to %s\n", chan, object_name->dsc$a_pointer);
#endif
    return 0;
}


int
bzero (buf, size)
    char *buf;
    int size;
{
    return (BZERO(buf, size));
}


#ifdef NOTDEF
/*
 * connect():
 * supports connecting to a "socket" given the network channel (from socket())
 */
int
connect (chan, name, namelen)
    unsigned short chan;
    struct sockaddr *name;
    unsigned int namelen;
{
    int        status;
    unsigned short    objlen;
    char *pncb;
    struct dsc$descriptor ncb_desc;
    static struct io_stat_blk iosb;
    struct dsc$descriptor_s *objdsc;
        static char *objstr = NULL;
        char hname[32];
        int l = 0;

#ifdef DNDEBUG
        fprintf(stderr, "connect: start - passed chan %d\n", chan);
#endif /* DNDEBUG */
        if (!objstr) objstr = DNETALLOC(NCBSIZE);
        if (!(pncb = initNCB(name, &objlen, TASKOBJECT, NULL))) 
        {
            char *a = objstr;
            short *area = &name->sa_data[2];
            short *node = &name->sa_data[4];

               objdsc = nameFromPort (name);
            BCOPY(TASKOBJECT, a, strlen(TASKOBJECT));
            a += strlen(TASKOBJECT);
            BCOPY(objdsc->dsc$a_pointer, a, strlen(objdsc->dsc$a_pointer));
            a+= strlen(objdsc->dsc$a_pointer);
            *a = 0x0;
            sprintf (hname, "%d.%d::", *area, *node);
            pncb = initNCB(name, &objlen, objstr, hname);
        }
        ncb_desc.dsc$w_length = objlen;
        ncb_desc.dsc$a_pointer = pncb;
        ncb_desc.dsc$b_dtype = DSC$K_DTYPE_T;
        ncb_desc.dsc$b_class = DSC$K_CLASS_S;

        status = SYS$QIOW (0, chan, IO$_ACCESS, &iosb, 0, 0, 0, &ncb_desc, 0, 0, 0, 0);
        if (!VMSFAIL(status)) status = iosb.status;
        if (!VMSFAIL(status))
    {
#ifdef DNDEBUG
        fprintf(stderr, "connect: Connection to %s on channel %d\n", pncb, chan);
#endif
            DNETFREE (pncb);
                return 0;
    }
        else
                LIB$SIGNAL (status);
    DNETFREE (pncb);
    return -1;
}
#endif /* NOTDEF */


/*
 * connect():
 * supports connecting to a "socket" given the network channel (from socket())
 */
int
connect (chan, name, namelen)
        unsigned short chan;
        struct sockaddr *name;
        unsigned int namelen;
{
        int             status;
        unsigned short  objlen;
        char *pncb;
        struct dsc$descriptor ncb_desc;
        static struct io_stat_blk iosb;
        struct dsc$descriptor_s *objdsc;
        char *objstr = DNETALLOC(NCBSIZE);
        char *hname;
        int l = 0;
        char *a;
        short *port = &name->sa_data[0];
        short *area = &name->sa_data[2];
        short *node = &name->sa_data[4];

#ifdef DNDEBUG
        fprintf(stderr, "connect: start - passed chan %d port %d addr %d.%d\n",
                chan, *port, *area, *node);
#endif /* DNDEBUG */
        BCOPY (TASKOBJECT, objstr, strlen(TASKOBJECT));
        hname = NULL;
        if ((objdsc = nameFromPort (name)))
        {
                hname = DNETALLOC(32);
                a = objstr + strlen(TASKOBJECT);
                BCOPY(objdsc->dsc$a_pointer, a, strlen(objdsc->dsc$a_pointer));
                a+= strlen(objdsc->dsc$a_pointer);
                *a = 0x0;
                sprintf (hname, "%d.%d::", *area, *node);
        }
        pncb = initNCB(name, &objlen, objstr, hname);
        if (hname) DNETFREE(hname);
        ncb_desc.dsc$w_length = objlen;
        ncb_desc.dsc$a_pointer = pncb;
        ncb_desc.dsc$b_dtype = DSC$K_DTYPE_T;
        ncb_desc.dsc$b_class = DSC$K_CLASS_S;

        status = SYS$QIOW (0, chan, IO$_ACCESS, &iosb, 0, 0, 0, &ncb_desc, 0, 0, 0, 0);
        if (!VMSFAIL(status)) status = iosb.status;
        if (!VMSFAIL(status))
        {
#ifdef DNDEBUG
                fprintf(stderr, "connect: Connection to %s on channel %d\n", pncb, chan);
#endif
                DNETFREE (pncb);
                return 0;
        }
        else
                LIB$SIGNAL (status);
        DNETFREE (pncb);
        return -1;
}


/*
 * ffs
 * Emulates the find first set function for UNIX.
 */
int
ffs (lw)
    int lw;
{
    int fp;
    int sp = 0;
    int status;
    int sz = sizeof(lw)*BITSPERBYTE;

    status = lib$ffs(&sp, &sz, &lw, &fp);
    if (VMSFAIL(status))
    {
        LIB$SIGNAL(status);
        exit();
    }
    return ++fp;
}


/*
 * getdtablesize
 * Routine to return the given size of the file descriptor table (channel table).
 */
int
getdtablesize()
{
    return FD_SETSIZE;  /* *** should be CHANNELCNT in SYSGEN */
}


/*
 * gethostbyaddr
 * Socket library emulation routine to return the host name given a (DECnet)
 * network address.
 */
struct hostent *
gethostbyaddr (addr, length, family)
    unsigned char *addr;
    unsigned int length;
    unsigned int family;
{
    static struct hostent *t = NULL;

#ifdef DNDEBUG
        fprintf(stderr, "gethostbyaddr: start\n");
#endif /* DNDEBUG */
        if (!t)
        {
            t = DNETALLOC (sizeof(struct hostent));
            t->h_name = DNETALLOC(NODNAMSIZ);
        t->h_aliases = NULL;
            t->h_addrtype = AF_DECnet; 
        t->h_length = sizeof(long);
        t->h_addr_list = DNETALLOC (sizeof(long));
        t->h_addr_list[0] = DNETALLOC (sizeof(long));
        t->h_cputype = DNETALLOC (strlen(CPUTYPE)+1);
        t->h_opsys = DNETALLOC (strlen(OPSYS)+1);
        bcopy (CPUTYPE, t->h_cputype, strlen(CPUTYPE));
        bcopy (OPSYS, t->h_opsys, strlen(OPSYS));
        }
#ifdef DNDEBUG
        fprintf (stderr, "gethostbyaddr: DECnet address %d.%d\n", 
            (short) *addr, (short) (*(addr+sizeof(short))));
#endif
        sprintf (t->h_name, "%d.%d", (short) *addr, (short) *(addr+sizeof(short)));
        if (NCPfind (0, t->h_addr_list[0], t->h_name) < 0)
        {
            h_errno = HOST_NOT_FOUND;
            return 0;
        }
    return t;
}


/*
 * gethostbyname
 * Routine to return the network address of a given node name.
 */
struct hostent *
gethostbyname (name)
    char *name;
{
    static struct hostent *t = NULL;

#ifdef DNDEBUG
        fprintf(stderr, "gethostbyname: start\n");
#endif /* DNDEBUG */
        if (!t)
        {
            t = DNETALLOC (sizeof(struct hostent));
            t->h_name = DNETALLOC(NODNAMSIZ);
        t->h_aliases = NULL;
        t->h_addrtype = AF_DECnet;
        t->h_length = sizeof(long);
        t->h_addr_list = DNETALLOC (sizeof(long));
        t->h_addr_list[0] = DNETALLOC (sizeof(long));
        t->h_cputype = DNETALLOC (strlen(CPUTYPE)+1);
        t->h_opsys = DNETALLOC (strlen(OPSYS)+1);
        bcopy (CPUTYPE, t->h_cputype, strlen(CPUTYPE));
        bcopy (OPSYS, t->h_opsys, strlen(OPSYS));
        }
    bcopy (name, t->h_name, strlen(name)+1);
        if (NCPfind (0, t->h_addr_list[0], t->h_name) < 0)
        {
            h_errno = HOST_NOT_FOUND;
            return 0;
        }
    return t;
}


/*
 * gethostname
 * Routine to return the host node name.
 */
int
gethostname (hname, l)
        char *hname;
        int l;
{
        short retlen = 0;

        unsigned short iosb[4];
        struct
            {
            unsigned short id_buflen;
            unsigned short id_itmcod;
            int *id_scsid;
            unsigned short *id_retlen;
            int listend;
            } itmlst = {l, SYI$_NODENAME, hname, &retlen, 0};

#ifdef DNDEBUG
        fprintf(stderr, "gethostname: start\n");
#endif /* DNDEBUG */
        SYS$GETSYIW (0, 0, 0, &itmlst, iosb, 0, 0);
        hname[retlen] = '\0';
    return 0;
}


/*
 * getpeername
 * Routine to return the node address (DECnet) associated with a given channel.
 */
int
getpeername (chan, psock, psock_l)
    unsigned short chan;
    struct sockaddr *psock;
    long *psock_l;
{
        unsigned short sChan = chan>>4;

#ifdef DNDEBUG
        fprintf(stderr, "getpeername: start\n");
#endif /* DNDEBUG */
    psock->sa_family = AF_DECnet;
        if (!ChanTableInfo[sChan]) return -1;
        *psock_l = sizeof(long);
    return NCPfind (sChan, psock, ChanTableInfo[sChan]->nodename);
}


/*
 * gettimeofday
 * Routine to return the time in secs and millisecs
 */
int
gettimeofday (timev)
    struct timeval *timev;
{
    static timeb_t t;

#ifdef DNDEBUG
        fprintf(stderr, "gettimeofday: start\n");
#endif /* DNDEBUG */
    ftime (&t);
    timev->tv_sec = t.time;
    timev->tv_usec = t.millitm * 1000;

    return 0;
}


int
htonl (i)
    int i;
{
        int t1;
    int t2;

        t2 = i; 
        ((char *)&t1)[0] = ((char *)&t2)[3];
    ((char *)&t1)[1] = ((char *)&t2)[2];
        ((char *)&t1)[2] = ((char *)&t2)[1];
    ((char *)&t1)[3] = ((char *)&t2)[0];

    return t1;
}


unsigned short
htons (i)
    unsigned short i;
{
        unsigned short t1;
    unsigned short t2;

        t2 = i;
        ((char *)&t1)[0] = ((char *)&t2)[1];
    ((char *)&t1)[1] = ((char *)&t2)[0];

        return t1;
}


/*
 * inet_addr
 * Routine to translate the network address into a longword value.
 */
int
inet_addr (address)
    char *address;
{
    char    *a;
    char    *b[4];
        char     *tmpvm;
    int    i;
    int    j = 0;
    int    out;

#ifdef DNDEBUG
        fprintf(stderr, "inet_addr: start\n");
#endif /* DNDEBUG */
        if(isalpha(*address)) return -1;
    b[j++] = address;
        tmpvm = DNETALLOC(strlen(address));
        bcopy (address, tmpvm, strlen(address));
    for (i = 0, a = tmpvm; i < strlen(address); i++, a++)
    {
        if ((*a == '.') || (*a = ':'))
        {
            *a = '\0';
            b[j++] = a + 1;
        }
    }
/* *** Needs to read all network address types (?) */
    a = (char *) &out;
    for (i = 0; i < j; i++, a++)
        *a = (char) atoi(b[i]);
    DNETFREE(tmpvm);
    return (htonl(out));
}


/*
 * listen():
 * Module to emulate a "listen" placed on the passed channel.  In the case of
 * DECnet, we'll interpret this to mean issue a read to the network command
 * mailbox associated with the NET: device.  Right now I don't really have a
 * use for the back log queue length, but we'll keep it around anyhow.
 */
int
listen (chan, bl)
    unsigned short chan;
    unsigned int bl;
{
    int    status;

#ifdef DNDEBUG
        fprintf(stderr, "listen: start - passed chan %d\n", chan);
#endif /* DNDEBUG */
        lisChannel = netChannel;
    ChanTableInfo[(chan>>4)]->backLogQueueLength = bl; /* *** */
    if (!ChanTableInfo[(chan>>4)]->readAstPend)
    {
        DISABLE_ASTS;        /* don't forget this or we're toast! */
        status = issueNetCmdRead (chan);
        ChanTableInfo[(chan>>4)]->readAstPend = 1;
        if (VMSFAIL(status))
        {
            LIB$SIGNAL (status);
            exit(0);
        }
    }
    return 0;
}


/*
 *
 * SELECT for DECnet
 * Note:
 *    readfds should contain the fd for the _NET: device for new connections.
 *     writefds will be returned from outstanding write AST notification.
 *
 * Return:
 *     int value specifying the number of channels ready.
 *
 */
int
select (width, readfds, writefds, exceptfds, timeout)
    int width;
    fd_set *readfds;
    fd_set *writefds;
    fd_set *exceptfds;
    struct timeval *timeout;
{
    int i;
    int j;
    int k;
    int lw;
    char *plw;
    int delta[2];
    int totset;
    int fd_group;
    int numBytes = width/NBBY;
    int status;
        int bit;
        int pos;
    fd_set readMask;
    fd_set writeMask;
    fd_set exceptMask;

    stackIndex = 0;
    timerExpired = 0;
    totset = 0;

#ifdef DNDEBUG
        fprintf(stderr, "select: start\n");
#endif /* DNDEBUG */
/*
 * Optimized channel listening setup loop.
 */
    for (fd_group = 0, i = 0, j = 0; j < width; ++fd_group)
    {
        if (readfds->bits[fd_group])
        {
                pos = -1;
            while( _FFS(pos+1, ((NBLW-1)-pos), &readfds->bits[fd_group], &pos))
                {
                    bit = j + pos;
                    if (!ChanTableInfo[bit])
                    {
                        errno = EBADF;
                        return -1;
                    }
                    if (!ChanTableInfo[bit]->readAstPend) 
                        status = issueRead ((bit<<4));
            }
        }
        else
            j += NBLW;
    }
    LIB$MOVC5(&zero, NULL, "\0", &numBytes, (char *) &readMask);
    if (writefds) LIB$MOVC5(&zero, NULL, "\0", &numBytes, (char *) &writeMask);
/*
    This is a future enhancement, currently omitted for time considerations:
    if (exceptfds) LIB$MOVC5(&zero, NULL, "\0", &numBytes, (char *) &exceptMask);
*/
    if (!stackIndex)
    {
        int sec = 0;
        int usec = 0;

        if (timeout)
        {
            sec = timeout->tv_sec;
            usec = timeout->tv_usec;
        }
        if ((timeout) && (!(sec + usec)))
            timerExpired = 1;
        else
        {
/*
 * Here's the juicy part of select().  Check for a timeout value to be setup. Then
 * enable ASTs, check for any I/O completion, and disable ASTs.
 */
            if (timeout)
            {
                delta[0] = (sec * -10000000) + (usec * -10);
                delta[1] = -1;
                SYS$SETIMR ( 0, delta, timeoutAst, timeout, 0);
            }
            ENABLE_ASTS;
            HIBERNATE;
            DISABLE_ASTS;
            if ((!timerExpired) && (timeout))
            {
                status = SYS$CANTIM (timeout, 0);
                if (VMSFAIL(status)) LIB$SIGNAL(status);
            }
        }
    }
    if (!stackIndex && !timerExpired)
        fprintf (stderr, "select: error - stackIndex 0 timerExpired 0\n");
/*
 * Now check the ast stack for completions.  Mark any channel's with I/O pending
 * on the internal fd masks.
 */
    if ((timerExpired) && (!stackIndex))
    {
        for (i = 0, lw = 0, fd_group = 0; lw < width; lw+=NBLW, fd_group++)
        {
            if (readfds)
                readfds->bits[fd_group] = 0;
            if (writefds)
                writefds->bits[fd_group] = 0;
            if (exceptfds)
                exceptfds->bits[fd_group] = 0;
        }
    }
    else
    {
#ifdef DNDEBUG
        fprintf(stderr, "select: stackIndex %d\n", stackIndex);
#endif
        for (i = 0; i < stackIndex; i++)
        {
/*
 *    Note, read_iosb.status = SS$_LINKABORT, when the line dies! 
 */
            if ((stack[i].type == NET_CMD) || (ChanTableInfo[(stack[i].channel>>4)]))
            {
            switch (stack[i].type)
            {
            case NET_CMD :
            {
                if (processNetCmd(&stack[i].channel))
                {
                    FD_SET(stack[i].channel, &readMask);
                    totset++;
                }
                break;
            }
            case NET_RD :
            {
                if (readfds)
                {
                    if (FD_ISSET(stack[i].channel, readfds))
                    {
                        FD_SET(stack[i].channel, &readMask);
                        totset++;
                    }
                }
            }
            case NET_WRT : 
            {
                if (writefds)
                {
                    if (FD_ISSET(stack[i].channel, writefds))
                    {
                        FD_SET(stack[i].channel, &writeMask);
                        totset++;
                    }
                }
            }
/*
 *            case NET_CMD : 
 *            {
 *                if (exceptfds)
 *                {
 *                    if (FD_ISSET(stack[i].channel, exceptfds))
 *                    {
 *                        FD_SET(stack[i].channel, &exceptMask);
 *                        totset++;
 *                    }
 *                }
 *            }
 */
            } /* switch */
            } /* LINKABORT if */
            else
            {
                errno = EBADF;
                totset = -1;
                break;
            }
        }
/*
 * Now "and" the two tables together to create the returned fd table(s).
 */
        for (lw = 0, fd_group = 0; lw < width; lw+=NBLW, ++fd_group)
        {
            if (readfds)
                readfds->bits[fd_group] &= readMask.bits[fd_group];
            if (writefds)
                writefds->bits[fd_group] &= writeMask.bits[fd_group];
            if (exceptfds)
                exceptfds->bits[fd_group] &= exceptMask.bits[fd_group];
        }
    }
#ifdef DNDEBUG
        fprintf(stderr, "select: returning %d\n", totset);
#endif
    return totset;
}


/*
 * setsockopt
 * Stub routine for emulation purposes right now.
 */
int
setsockopt (chan, level, optname, optval, optlen)
    unsigned short chan;
    unsigned long level;
    unsigned long optname;
    char *optval;
    unsigned long optlen;
{
#ifdef DNDEBUG
        fprintf(stderr, "setsockopt: start - passed chan %d\n", chan);
#endif /* DNDEBUG */
    ChanTableInfo[(chan>>4)]->o[optname].option = optval;
    ChanTableInfo[(chan>>4)]->o[optname].optionlen = optlen;
    return 0;
}


/*
 * socket():
 * Two channels are created as a result of this call,
 * 1) mbxChannel - the network command mailbox throught which all low level 
 *           connection information and management crap flows
 * 2) netChannel - the channel to the NET device, used by a server to est. a 
 *           DECnet object; used by a client to connect and perform I/O on
 *           the aforementioned object.
 * Note: only netChannel is "seen" by the calling layer program.
 */
unsigned short
socket (addrfam, type, proto)
    unsigned long addrfam;
    unsigned long type;
    unsigned long proto;
{
    int status;

#ifdef DNDEBUG
        fprintf(stderr, "socket: start\n");
#endif /* DNDEBUG */
        connectNet ();
    if (!ChanTableInfo[(netChannel>>4)]) clearChanTable((netChannel>>4));
    if (!ChanTableInfo[(mbxChannel>>4)]) clearChanTable((mbxChannel>>4));
    return netChannel;
}


/*
 * socket_close
 * Emulation routine for closing channels.
 */
int
socket_close (chan)
        unsigned short chan;
{
    unsigned short shiftChan = chan>>4;

#ifdef DNDEBUG
    fprintf(stderr, "socket_close: linkTerm %d\n", chan);
#endif
    linkTerm (shiftChan);
    clearChanTable (shiftChan);
    return 0;
}


/*
 * socket_ioctl
 * Emualtion routine for stubbing purposes only.
 */
int
socket_ioctl (chan, request, argp)
    unsigned short chan;
    unsigned int request;
    char *argp;
{
#ifdef DNDEBUG
        fprintf(stderr, "socket_ioctl: start - passed chan %d\n", chan);
#endif /* DNDEBUG */
    if (chan > FD_SETSIZE)
        return -1;
    ChanTableInfo[(chan>>4)]->type = request;
}


/*
 * socket_perror
 * Emulation for the same.  Relatively weak right now in that it ignores errno
 * values.
 */
void
socket_perror (s)
    char *s;
{
#ifdef DNDEBUG
        fprintf(stderr, "socket_perror: start\n");
#endif /* DNDEBUG */
    fprintf(stdout, "%s", s);
}


/*
 * socket_read (chan, target, length)
 * Routine to emulate the a streamed read on a socket.  Currently the read
 * supports ONLY nonblocking I/O.  That is, the read will complete is there
 * is ANYTHING in the channel buffer not depending on the passed "length" field.
 * With blocking I/O, the read will wait until the length field request has
 * been statisfied.
 */
int
socket_read (chan, target, l)
        unsigned short   chan;
        char    *target;
        int     l;
{
    int    shiftChan = chan>>4;
    char *vmbase = ChanTableInfo[shiftChan]->base;
    char *vmdelta = ChanTableInfo[shiftChan]->delta;
    int vmlen = ChanTableInfo[shiftChan]->len;
        int     remainder;
        char    *savtarget;
        int     movetot;
    int    pass;
    int    status;
    struct io_stat_blk iosb;

#ifdef DNDEBUG
        fprintf(stderr, "socket_read: start - passed chan %d\n", chan);
#endif /* DNDEBUG */
    if (!ChanTableInfo[shiftChan]) return -1;
        if (!l) return l;

        savtarget = target;
        movetot = 0;
/* Check the size of the channel's read buffer             */
        remainder = (vmbase + vmlen) - vmdelta;
/*
 * If this routine was called after an AST read notification from select() and the
 * read in buffer size was 0, return 0 now so that a QIO is *not* performed.  In the
 * case of a calling program that does *not* use select/AST's, readAstDone will remain 0.
 */
        if (ChanTableInfo[shiftChan]->readAstDone)
        {
            ChanTableInfo[shiftChan]->readAstDone = 0;
            if(!remainder) return 0;
        }
/*
 * See if the VM area exists and, if so, is there anything in there?  If yes again,
 * move it, update VM pointers, and return. (Non blocking I/O).
 */
        if (remainder)
        {
        pass = MIN(remainder, l);
                bcopy (vmdelta, target, pass);
                movetot += pass;
                savtarget += pass;
                vmdelta += pass;
        ChanTableInfo[shiftChan]->base = vmbase;
        ChanTableInfo[shiftChan]->delta = vmdelta;
        ChanTableInfo[shiftChan]->len = vmlen;
        return movetot;
        }
/*
 * VM memory area is empty, so fill it up and update the pointers.
 */
    if (!remainder)
    {
                status = SYS$QIOW (0, chan, IO$_READVBLK, &iosb, 0, 0, vmbase,
                        MAX_QIO, 0, 0, 0, 0);
                if (!(status & STS$M_SUCCESS) || !(iosb.status & STS$M_SUCCESS))
                        return -1;
        vmdelta = vmbase;
                vmlen = iosb.msg_len;
#ifdef DNDEBUG
            fprintf(stderr,"socket_read: iosb.len %d on channel %d\n",iosb.msg_len, chan);
#endif
        if (vmlen < l) fprintf(stderr, "socket_read: returned < request %d/%d\n", vmlen, l);
        }
    pass = MIN(vmlen, l);
        bcopy (vmdelta, savtarget, pass);
        movetot += pass;
        vmdelta += pass;
    ChanTableInfo[shiftChan]->base = vmbase;
    ChanTableInfo[shiftChan]->delta = vmdelta;
    ChanTableInfo[shiftChan]->len = vmlen;
    return movetot;
}


/*
 * socket_write
 * Emulation routine for writes to a given channel.  Note that I tried IO$M_NOW
 * but it had the effect of causing an error (overflow I believe).  
 * Does buffering occur?  Is it detrimental to the works since I don't check for
 * write completion?
 */
int
socket_write (chan, buffer, l)   /* Issue writes, (IO$_WRITEVBLK), over the logical link.*/
        unsigned short    chan;
        char            *buffer;
        int             l;
{
    int status;
    struct io_stat_blk iosb;
    
#ifdef DNDEBUG
        fprintf(stderr, "socket_write: start - passed chan %d\n", chan);
#endif /* DNDEBUG */
        status = SYS$QIOW (0, chan, IO$_WRITEVBLK, &iosb, 0, 0, buffer,
                        l, 0, 0, 0, 0);
        if (VMSFAIL(status)) 
    {
        LIB$SIGNAL(status);
        return -1;
    }
        if (VMSFAIL(iosb.status)) 
    {
        LIB$SIGNAL(iosb.status);
        return -1;
    }
#ifdef DNDEBUG
    fprintf(stderr, "socket_write: wrote %d/%d to channel %d\n",iosb.msg_len, l, chan);
#endif
    return iosb.msg_len;
}


/*
 * socket_writev
 * Emulation routine for the ugly iov style socket writing.
 */
int
socket_writev( d, iov, iovcnt)
        int d;
        struct iovec *iov;
        int iovcnt;
{
        int     totbytes;
        int     bytes;
        int     i;

#ifdef DNDEBUG
        fprintf(stderr, "socket_writev: start - passed chan %d\n", d);
#endif /* DNDEBUG */
        for(i = 0, totbytes = 0; i < iovcnt; i++)
        {
                bytes = socket_write((short) d, iov[i].iov_base, MIN(MAX_QIO, iov[i].iov_len));
                totbytes += bytes;
                if(bytes < iov[i].iov_len)
                {
#ifdef DNDEBUG
                        fprintf(stderr, "Block occured in writev, %d/%d\n",bytes,
                                iov[i].iov_len);
#endif
                        socket_errno = EWOULDBLOCK;
                        return totbytes;
                }
        }
        return totbytes;
}
