/*
 * $NCDId: @(#)emulationlib.h,v 4.4 1992/01/31 11:28:04 hardison Exp $
 * 
 * Emulation defines, strutures and macros for the DNET_SOCKET_LIBRARY
 * OBH - NCD - 3/2/92
 */
#include "nfbdef.h"

#ifndef _TIME_
#define _TIME_
#endif /*_TIME_*/

typedef unsigned short u_short;
typedef unsigned long u_long;

#define BCMP(s1, s2, l) (memcmp ((s1), (s2), (l)))
#define BCOPY(s, d, l) (memcpy ((d), (s), (l)))
#define BZERO(b,len) memset(b, 0, (size_t)(len))
#define DNETALLOC(s)	(dnMalloc((s)))
#define DNETFREE(a)	(dnFree((a)))
#define ENABLE_ASTS     (SYS$SETAST(1))
#define HIBERNATE       (SYS$HIBER())
#define DISABLE_ASTS    (SYS$SETAST(0))
#define WAKE_UP_DAMMIT	(SYS$WAKE(0, 0))
#define MAX_QIO         0xffff
#define VMSFAIL(status) (0==(1&(status)))
#define BITSPERBYTE      8
#define MAX_OPTION      4
#define MAX_LINKS       32
#define NODNAMSIZ	16
#define NET_RD          1
#define NET_WRT         2
#define NET_CMD         3
#define TEMP_MBX        0
#define MAX_MSG         128
#define BUF_QUO         128
#define EWOULDBLOCK     35
#define TASKOBJECT	"TASK="
#define MAXDNETNODES	30
#define HOST_NOT_FOUND  1
#define NFB_MAX_FIELD_ID_ENTRIES 10
#define AF_DECnet	12
#define AF_INET         2
#define DNET_PREFIX     "DECNET/"
#define DNET_IV_MAXLEN  8
#define NCBSIZE         256
#define DECNET 		"DECnet"
#define CPUTYPE 	"Vax"
#define OPSYS   	"VMS"
#define INADDR_ANY              (u_long)0x00000000
#define FIONBIO 	126
#define SOCK_STREAM     1      
#define SOL_SOCKET      0xffff 
#define SO_REUSEADDR    0x0004
#define SO_USELOOPBACK  0x0040

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif /* MIN */

#ifndef MAX
#define MAX(a,b) ((a) < (b) ? (b) : (a))
#endif /* MAX */

struct io_stat_blk
{
        short int status;
        unsigned short msg_len;
        int unused;
};

struct astblk
{
    	char type;
        unsigned short chan;
        char unused;
};

typedef union
{
        int astid;
	struct astblk ast;
} astParam;

struct
{
        char func;
        int terminator;
} nfb = { NFB$C_DECLNAME, 0 };

struct in_addr {
        u_long s_addr;
};

struct sockaddr_in {
        short   sin_family;
        u_short sin_port;
        struct  in_addr sin_addr;
        char    sin_zero[8];
};

struct sockaddr
{
        unsigned short sa_family;
        char    sa_data[14];
};

struct hostent
{
        char    *h_name;
        char    **h_aliases;
        int     h_addrtype;
        int     h_length;
        char    **h_addr_list;
#define h_addr h_addr_list[0]
        char    *h_cputype;
        char    *h_opsys;
        char    **h_protos;
        struct sockaddr *h_addresses;
};

struct stackstruct
{
        short   type;
        unsigned short channel;
};

/*
 * TYPES.H emulation
 */
#define NBBY    8               	/* number of bits in a byte */
#define NBLW    (NBBY*sizeof(long))	/* number of bits in a longword */
#ifndef FD_SETSIZE
#define FD_SETSIZE 512
#endif
#define CHANNELSIZE     16      /* Size of a channel */

typedef long    fd_mask;
#define NFDBITS (sizeof(fd_mask) * NBBY)        /* bits per mask */
#ifndef howmany
#define howmany(x, y)   (((x)+((y)-1))/(y))
#endif

typedef struct fd_set {
        fd_mask bits[howmany(FD_SETSIZE, NFDBITS)];
} fd_set;

#define FD_SET(n, p)    ((p)->bits[(n)/CHANNELSIZE/NFDBITS] \
                        |= (1 << (((n)/CHANNELSIZE) %NFDBITS)))
#define FD_CLR(n, p)    ((p)->bits[(n)/CHANNELSIZE/NFDBITS] \
                        &= ~(1 << (((n)/CHANNELSIZE) %NFDBITS)))
#define FD_ISSET(n, p)  ((!((n) % CHANNELSIZE)) && ((p)->bits[(n)/CHANNELSIZE/NFDBITS] \
                        & (1 <<(((n)/CHANNELSIZE) % NFDBITS))))
#define FD_ZERO(p)      bzero((char *)(p), sizeof(*(p)))

/*
 * Structure returned by gettimeofday(2) system call,
 * and used in other calls.
 */
struct timeval
{
        long    tv_sec;         /* seconds */
        long    tv_usec;        /* and microseconds */
};

struct iovec
{
        char    *iov_base;
        int     iov_len;
};
/*
 * The channel table is needed to keep track of each connection's
 * virtual memory read buffer base address, current (delta) pointer, and
 * current size.  In addition, the structure provides a request reference
 * variable usde mainly to determine the type of connection (NONBLOCKING or otherwise).
 * In the future, this mechanism can be used to expand the types of connections
 * allowed for socket I/O.
 */
struct opttable
{
        char    *option;
        int     optionlen;
};

struct chantable
{
        char    *base;    	      	/* base of virt mem for reads */
        char    *delta;   	      	/* current ptr of virt mem for reads */
	char	*nodename;		/* name of attached client node */
	char	*user;			/* user of attached client */
    	short	area;			/* DECnet address area number */
    	short	node;			/* DECnet address node number */
        short   readAstPend;    	/* 0 = no AST's pending */
        short   readAstDone;    	/* 1 = AST read data available */
	int	chanDassgn;		/* Channel deassign flag for asynch deallocs */
	int	unit;			/* Network unit number from GETDVI */
        int     len;           		/* current len of virt mem */
        int     type;           	/* channel request type from socket_ioctl */
        int     *backLogQueue;  	/* Non-NULL for "accept"ing (_NET:) channels only */
        int     backLogQueueLength;
	struct io_stat_blk read_iosb;	/* IOSB for QIO read returns */
	struct io_stat_blk write_iosb;	/* IOSB for QIO write returns */
        struct opttable o[MAX_OPTION];
};

/* *** should be linked to MAXCLIENTS (from site.h or globals.h)*/

typedef struct
{
    	char	*nodebuf;
    	short	area;
    	short	node;
} NCPstruct;

struct nfbdef 
{
    unsigned char	fct;
    unsigned char	flags;
    unsigned char	database;
    unsigned char	oper;
    long 		srchKey;
    long 		srch2Key;
    unsigned char	oper2;
    unsigned char	mbz1;
    short 		cellSize;
    long		fldid[NFB_MAX_FIELD_ID_ENTRIES];
};

struct nfbdef1 
{
    union 
    {
    	long	paramId;
    	struct 
    	{
    		unsigned 	inx : 16;
    		unsigned 	typ : 2;
    		unsigned 	spare : 6;
    		unsigned 	db : 8;
    	} fields;
    } overlay;
};

struct nfbdef2 
{
    union 
    {	
    	long	lngValue;
    	long	bitValue;
    	struct 
    	{
    		short 	 	strCount;
    		char 		strText;
    	} string;
    } overlay;
};

struct netbuf
{
        short netCmd;
        short unit;
        char mbxMsg[MAX_MSG];
} nb;

