// btree version 2u  sched_yield locks
//    with combined latch & pool manager
//    and phase-fair reader writer lock
// 12 MAR 2014

// author: karl malbrain, malbrain@cal.berkeley.edu

/*
This work, including the source code, documentation
and related data, is placed into the public domain.

The orginal author is Karl Malbrain.

THIS SOFTWARE IS PROVIDED AS-IS WITHOUT WARRANTY
OF ANY KIND, NOT EVEN THE IMPLIED WARRANTY OF
MERCHANTABILITY. THE AUTHOR OF THIS SOFTWARE,
ASSUMES _NO_ RESPONSIBILITY FOR ANY CONSEQUENCE
RESULTING FROM THE USE, MODIFICATION, OR
REDISTRIBUTION OF THIS SOFTWARE.
*/

// Please see the project home page for documentation
// code.google.com/p/high-concurrency-btree

#ifndef BTREE_H
#define BTREE_H

#include "include/data_types.h"

#define BT_ro 0x6f72      // ro
#define BT_rw 0x7772      // rw
#define BT_fl 0x6c66      // fl

#define BT_maxbits 15                    // maximum page size in bits
#define BT_minbits 12                    // minimum page size in bits
#define BT_minpage (1 << BT_minbits)     // minimum page size
#define BT_maxpage (1 << BT_maxbits)     // maximum page size

// BTree page number constants

#define ALLOC_page 0
#define ROOT_page  1
#define LEAF_page  2
#define LATCH_page 3

// Number of levels to create in a new BTree

#define MIN_lvl 2
#define MAX_lvl 15

/*
There are five lock types for each node in three independent sets: 
1. (set 1) AccessIntent: Sharable. Going to Read the node. Incompatible with NodeDelete. 
2. (set 1) NodeDelete: Exclusive. About to release the node. Incompatible with AccessIntent. 
3. (set 2) ReadLock: Sharable. Read the node. Incompatible with WriteLock. 
4. (set 2) WriteLock: Exclusive. Modify the node. Incompatible with ReadLock and other WriteLocks. 
5. (set 3) ParentModification: Exclusive. Change the node's parent keys. Incompatible with another ParentModification. 
*/

typedef enum{
    BtLockAccess,
    BtLockDelete,
    BtLockRead,
    BtLockWrite,
    BtLockParent
} BtLock;

// definition for latch implementation

volatile typedef struct {
    ushort lock[1];
} BtSpinLatch;

#define XCL   1
#define PEND  2
#define BOTH  3
#define SHARE 4

volatile typedef struct {
    ushort rin[1];       // readers in count
    ushort rout[1];      // readers out count
    ushort serving[1];   // writers out count
    ushort ticket[1];    // writers in count
} RWLock;

// define bits at bottom of rin

#define PHID 0x1    // writer phase (0/1)
#define PRES 0x2    // writer present
#define MASK 0x3    // both write bits
#define RINC 0x4    // reader increment

// Define the length of the page and key pointers

#define BtId 6

// Page key slot definition.

// If BT_maxbits is 15 or less, you can save 2 bytes
// for each key stored by making the first two uints
// into ushorts.  You can also save 4 bytes by removing
// the tod field from the key.

// Keys are marked dead, but remain on the page until
// cleanup is called. The fence key (highest key) for
// the page is always present, even if dead.

typedef struct {
#ifdef USETOD
    uint tod;                     // time-stamp for key
#endif
    ushort off:BT_maxbits;        // page offset for key start
    ushort dead:1;                // set for deleted key
    unsigned char id[BtId];       // id associated with key
} BtSlot;

// The key structure occupies space at the upper end of
// each page.  It's a length byte followed by the value
// bytes.

typedef struct {
    unsigned char len;
    unsigned char key[0];
} *BtKey;

// The first part of an index page.
// It is immediately followed
// by the BtSlot array of keys.

typedef struct BtPage_ {
    uint cnt;                     // count of keys in page
    uint act;                     // count of active keys
    uint min;                     // next key offset
    unsigned char bits:6;         // page size in bits
    unsigned char free:1;         // page is on free list
    unsigned char dirty:1;        // page is dirty in cache
    unsigned char lvl:6;          // level of page
    unsigned char kill:1;         // page is being deleted
    unsigned char clean:1;        // page needs cleaning
    unsigned char right[BtId];    // page number to right
} *BtPage;

typedef struct {
    struct BtPage_ alloc[2];      // next & free page_nos in right ptr
    BtSpinLatch lock[1];          // allocation area lite latch
    volatile uint latchdeployed;  // highest number of latch entries deployed
    volatile uint nlatchpage;     // number of latch pages at BT_latch
    volatile uint latchtotal;     // number of page latch entries
    volatile uint latchhash;      // number of latch hash table slots
    volatile uint latchvictim;    // next latch hash entry to examine
    volatile uint safelevel;      // safe page level in cache
    volatile uint cache[MAX_lvl]; // cache census counts by btree level
} BtLatchMgr;

// latch hash table entries

typedef struct {
    volatile uint slot;      // Latch table entry at head of collision chain
    BtSpinLatch latch[1];    // lock for the collision chain
} BtHashEntry;

// latch manager table structure

typedef struct {
    volatile uid page_no;    // latch set page number on disk
    RWLock readwr[1];        // read/write page lock
    RWLock access[1];        // Access Intent/Page delete
    RWLock parent[1];        // Posting of fence key in parent
    volatile ushort pin;     // number of pins/level/clock bits
    volatile uint next;      // next entry in hash table chain
    volatile uint prev;      // prev entry in hash table chain
} BtLatchSet;

#define CLOCK_mask 0xe000
#define CLOCK_unit 0x2000
#define PIN_mask 0x07ff
#define LVL_mask 0x1800
#define LVL_shift 11

//    The object structure for Btree access

typedef struct _BtDb {
    uint page_size;          // each page size    
    uint page_bits;          // each page size in bits    
    uid page_no;             // current page number    
    uid cursor_page;         // current cursor page number    
    int  err;
    uint mode;               // read-write mode
    BtPage cursor;           // cached frame for start/next (never mapped)
    BtPage frame;            // spare frame for the page split (never mapped)
    BtPage page;             // current mapped page in buffer pool
    BtLatchSet *latch;       // current page latch
    BtLatchMgr *latchmgr;    // mapped latch page from allocation page
    BtLatchSet *latchsets;   // mapped latch set from latch pages
    unsigned char *pagepool; // cached page pool set
    BtHashEntry *table;      // the hash table
#ifdef unix
    int idx;
#else
    HANDLE idx;
    HANDLE halloc;           // allocation and latch table handle
#endif
    unsigned char *mem;      // frame, cursor, memory buffers
    uint found;              // last deletekey found key
} BtDb;

typedef enum {
    BTERR_ok = 0,
    BTERR_notfound,
    BTERR_struct,
    BTERR_ovflw,
    BTERR_read,
    BTERR_lock,
    BTERR_hash,
    BTERR_kill,
    BTERR_map,
    BTERR_wrt,
    BTERR_eof
} BTERR;

// B-Tree functions

extern void bt_close(BtDb *bt);
extern uid bt_uid(BtDb *bt, uint slot);
extern uint bt_tod(BtDb *bt, uint slot);
extern BtKey bt_key(BtDb *bt, uint slot);
extern uint bt_nextkey(BtDb *bt, uint slot);
extern uid bt_findkey(BtDb *bt, unsigned char *key, uint len);
extern uint bt_startkey(BtDb *bt, unsigned char *key, uint len);
extern BtDb *bt_open(char *name, uint mode, uint bits, uint cacheblk);
extern BTERR bt_deletekey(BtDb *bt, unsigned char *key, uint len, uint lvl);
extern BTERR bt_insertkey(BtDb *bt, unsigned char *key, uint len, uint lvl, uid id, uint tod);

#endif

