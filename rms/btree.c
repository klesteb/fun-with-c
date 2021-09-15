// btree version 2u  sched_yield locks
//	with combined latch & pool manager
//	and phase-fair reader writer lock
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

#define _FILE_OFFSET_BITS 64
#define _LARGEFILE64_SOURCE

#ifdef linux
#define _GNU_SOURCE
#endif

#ifdef unix
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <sched.h>
#else
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <io.h>
#endif

#include <memory.h>
#include <string.h>

#include "btree.h"

//	internal functions

void bt_update (BtDb *bt, BtPage page);
BtPage bt_mappage (BtDb *bt, BtLatchSet *latch);

//  Helper functions to return slot values

extern BtKey bt_key (BtDb *bt, uint slot);
extern uid bt_uid (BtDb *bt, uint slot);
#ifdef USETOD
extern uint bt_tod (BtDb *bt, uint slot);
#endif

//  The page is allocated from low and hi ends.
//  The key offsets and row-id's are allocated
//  from the bottom, while the text of the key
//  is allocated from the top.  When the two
//  areas meet, the page is split into two.

//  A key consists of a length byte, two bytes of
//  index number (0 - 65534), and up to 253 bytes
//  of key value.  Duplicate keys are discarded.
//  Associated with each key is a 48 bit row-id.

//  The b-tree root is always located at page 1.
//	The first leaf page of level zero is always
//	located on page 2.

//	The b-tree pages are linked with right
//	pointers to facilitate enumerators,
//	and provide for concurrency.

//	When to root page fills, it is split in two and
//	the tree height is raised by a new root at page
//	one with two keys.

//	Deleted keys are marked with a dead bit until
//	page cleanup The fence key for a node is always
//	present, even after deletion and cleanup.

//  Deleted leaf pages are reclaimed  on a free list.
//	The upper levels of the btree are fixed on creation.

//  To achieve maximum concurrency one page is locked at a time
//  as the tree is traversed to find leaf key in question. The right
//  page numbers are used in cases where the page is being split,
//	or consolidated.

//  Page 0 (ALLOC page) is dedicated to lock for new page extensions,
//	and chains empty leaf pages together for reuse.

//	Parent locks are obtained to prevent resplitting or deleting a node
//	before its fence is posted into its upper level.

//	A special open mode of BT_fl is provided to safely access files on
//	WIN32 networks. WIN32 network operations should not use memory mapping.
//	This WIN32 mode sets FILE_FLAG_NOBUFFERING and FILE_FLAG_WRITETHROUGH
//	to prevent local caching of network file contents.

//	Access macros to address slot and key values from the page.
//	Page slots use 1 based indexing.

#define slotptr(page, slot) (((BtSlot *)(page+1)) + (slot-1))
#define keyptr(page, slot) ((BtKey)((unsigned char*)(page) + slotptr(page, slot)->off))

void bt_putid(unsigned char *dest, uid id)
{
int i = BtId;

	while( i-- )
		dest[i] = (unsigned char)id, id >>= 8;
}

uid bt_getid(unsigned char *src)
{
uid id = 0;
int i;

	for( i = 0; i < BtId; i++ )
		id <<= 8, id |= *src++; 

	return id;
}

BTERR bt_abort (BtDb *bt, BtPage page, uid page_no, BTERR err)
{
BtKey ptr;

	fprintf(stderr, "\n Btree2 abort, error %d on page %.8x\n", err, (unsigned int)page_no);
	fprintf(stderr, "level=%d kill=%d free=%d cnt=%x act=%x\n", page->lvl, page->kill, page->free, page->cnt, page->act);
	ptr = keyptr(page, page->cnt);
	fprintf(stderr, "fence='%.*s'\n", ptr->len, ptr->key);
	fprintf(stderr, "right=%.8x\n", (unsigned int)bt_getid(page->right));
	return bt->err = err;
}

//	Phase-Fair reader/writer lock implementation

void WriteLock (RWLock *lock)
{
ushort w, r, tix;

#ifdef unix
	tix = __sync_fetch_and_add (lock->ticket, 1);
#else
	tix = _InterlockedExchangeAdd16 (lock->ticket, 1);
#endif
	// wait for our ticket to come up

	while( tix != lock->serving[0] )
#ifdef unix
		sched_yield();
#else
		SwitchToThread ();
#endif

	w = PRES | (tix & PHID);
#ifdef  unix
	r = __sync_fetch_and_add (lock->rin, w);
#else
	r = _InterlockedExchangeAdd16 (lock->rin, w);
#endif
	while( r != *lock->rout )
#ifdef unix
		sched_yield();
#else
		SwitchToThread();
#endif
}

void WriteRelease (RWLock *lock)
{
#ifdef unix
	__sync_fetch_and_and (lock->rin, ~MASK);
#else
	_InterlockedAnd16 (lock->rin, ~MASK);
#endif
	lock->serving[0]++;
}

void ReadLock (RWLock *lock)
{
ushort w;
#ifdef unix
	w = __sync_fetch_and_add (lock->rin, RINC) & MASK;
#else
	w = _InterlockedExchangeAdd16 (lock->rin, RINC) & MASK;
#endif
	if( w )
	  while( w == (*lock->rin & MASK) )
#ifdef unix
		sched_yield ();
#else
		SwitchToThread ();
#endif
}

void ReadRelease (RWLock *lock)
{
#ifdef unix
	__sync_fetch_and_add (lock->rout, RINC);
#else
	_InterlockedExchangeAdd16 (lock->rout, RINC);
#endif
}

//	Spin Latch Manager

//	wait until write lock mode is clear
//	and add 1 to the share count

void bt_spinreadlock(BtSpinLatch *latch)
{
ushort prev;

  do {
#ifdef unix
	prev = __sync_fetch_and_add (latch->lock, SHARE);
#else
	prev = _InterlockedExchangeAdd16(latch->lock, SHARE);
#endif
	//  see if exclusive request is granted or pending

	if( !(prev & BOTH) )
		return;
#ifdef unix
	prev = __sync_fetch_and_add (latch->lock, -SHARE);
#else
	prev = _InterlockedExchangeAdd16(latch->lock, -SHARE);
#endif
#ifdef  unix
  } while( sched_yield(), 1 );
#else
  } while( SwitchToThread(), 1 );
#endif
}

//	wait for other read and write latches to relinquish

void bt_spinwritelock(BtSpinLatch *latch)
{
ushort prev;

  do {
#ifdef  unix
	prev = __sync_fetch_and_or(latch->lock, PEND | XCL);
#else
	prev = _InterlockedOr16(latch->lock, PEND | XCL);
#endif
	if(( !(prev & XCL) )) {
	  if(( !(prev & ~BOTH) )) {
		return;
	  } else {
#ifdef unix
		__sync_fetch_and_and (latch->lock, ~XCL);
#else
		_InterlockedAnd16(latch->lock, ~XCL);
#endif
      }
    }
#ifdef  unix
  } while(( sched_yield(), 1 ));
#else
  } while( SwitchToThread(), 1 );
#endif
}

//	try to obtain write lock

//	return 1 if obtained,
//		0 otherwise

int bt_spinwritetry(BtSpinLatch *latch)
{
ushort prev;

#ifdef  unix
	prev = __sync_fetch_and_or(latch->lock, XCL);
#else
	prev = _InterlockedOr16(latch->lock, XCL);
#endif
	//	take write access if all bits are clear

	if(( !(prev & XCL) )) {
	  if(( !(prev & ~BOTH) )) {
		return 1;
	  } else {
#ifdef unix
		__sync_fetch_and_and (latch->lock, ~XCL);
#else
		_InterlockedAnd16(latch->lock, ~XCL);
#endif
      }
    }
	return 0;
}

//	clear write mode

void bt_spinreleasewrite(BtSpinLatch *latch)
{
#ifdef unix
	__sync_fetch_and_and(latch->lock, ~BOTH);
#else
	_InterlockedAnd16(latch->lock, ~BOTH);
#endif
}

//	decrement reader count

void bt_spinreleaseread(BtSpinLatch *latch)
{
#ifdef unix
	__sync_fetch_and_add(latch->lock, -SHARE);
#else
	_InterlockedExchangeAdd16(latch->lock, -SHARE);
#endif
}

//	read page from permanent location in Btree file

BTERR bt_readpage (BtDb *bt, BtPage page, uid page_no)
{

#ifdef unix
	if( pread (bt->idx, page, bt->page_size, page_no << bt->page_bits) < bt->page_size ) {
		fprintf (stderr, "Unable to read page %.8x errno = %d\n", (unsigned int)page_no, errno);
		return bt->err = BTERR_read;
	}
#else
off64_t off = page_no << bt->page_bits;
OVERLAPPED ovl[1];
uint amt[1];

	memset (ovl, 0, sizeof(OVERLAPPED));
	ovl->Offset = off;
	ovl->OffsetHigh = off >> 32;

	if( !ReadFile(bt->idx, page, bt->page_size, amt, ovl)) {
		fprintf (stderr, "Unable to read page %.8x GetLastError = %d\n", page_no, GetLastError());
		return bt->err = BTERR_read;
	}
	if( *amt <  bt->page_size ) {
		fprintf (stderr, "Unable to read page %.8x GetLastError = %d\n", page_no, GetLastError());
		return bt->err = BTERR_read;
	}
#endif
	return 0;
}

//	write page to permanent location in Btree file
//	clear the dirty bit

BTERR bt_writepage (BtDb *bt, BtPage page, uid page_no)
{
off64_t off = page_no << bt->page_bits;

#ifdef unix
	page->dirty = 0;

	if( pwrite(bt->idx, page, bt->page_size, off) < bt->page_size )
		return bt->err = BTERR_wrt;
#else
OVERLAPPED ovl[1];
uint amt[1];

	memset (ovl, 0, sizeof(OVERLAPPED));
	ovl->Offset = off;
	ovl->OffsetHigh = off >> 32;
	page->dirty = 0;

	if( !WriteFile(bt->idx, page, bt->page_size, amt, ovl) )
		return bt->err = BTERR_wrt;

	if( *amt <  bt->page_size )
		return bt->err = BTERR_wrt;
#endif
	return 0;
}

//	link latch table entry into head of latch hash table

BTERR bt_latchlink (BtDb *bt, uint hashidx, uint slot, uid page_no)
{
BtPage page = (BtPage)((uid)slot * bt->page_size + bt->pagepool);
BtLatchSet *latch = bt->latchsets + slot;
int lvl;

	if(( latch->next = bt->table[hashidx].slot )) {
		bt->latchsets[latch->next].prev = slot;
    }

	bt->table[hashidx].slot = slot;
	latch->page_no = page_no;
	latch->prev = 0;
	latch->pin = 1;

	if(( bt_readpage (bt, page, page_no) )) {
		return bt->err;
    }

	lvl = page->lvl << LVL_shift;
	if(( lvl > LVL_mask )) {
		lvl = LVL_mask;
    }
	latch->pin |= lvl;		// store lvl
	latch->pin |= lvl << 3; // initialize clock

#ifdef unix
	__sync_fetch_and_add (&bt->latchmgr->cache[page->lvl], 1);
#else
	_InterlockedExchangeAdd(&bt->latchmgr->cache[page->lvl], 1);
#endif
	return bt->err = 0;
}

//	release latch pin

void bt_unpinlatch (BtLatchSet *latch)
{
#ifdef unix
	__sync_fetch_and_add(&latch->pin, -1);
#else
	_InterlockedDecrement16 (&latch->pin);
#endif
}

//	find existing latchset or inspire new one
//	return with latchset pinned

BtLatchSet *bt_pinlatch (BtDb *bt, uid page_no)
{
uint hashidx = page_no % bt->latchmgr->latchhash;
BtLatchSet *latch;
uint slot, idx;
uint lvl, cnt;
BtPage page;

  //  try to find our entry

  bt_spinwritelock(bt->table[hashidx].latch);

  if(( slot = bt->table[hashidx].slot )) do
  {
	latch = bt->latchsets + slot;
	if(( page_no == latch->page_no )) {
		break;
    }
  } while(( slot = latch->next ));

  //  found our entry
  //  increment clock

  if(( slot )) {
	latch = bt->latchsets + slot;
	lvl = (latch->pin & LVL_mask) >> LVL_shift;
	lvl *= CLOCK_unit * 2;
	lvl |= CLOCK_unit;
#ifdef unix
	__sync_fetch_and_add(&latch->pin, 1);
	__sync_fetch_and_or(&latch->pin, lvl);
#else
	_InterlockedIncrement16 (&latch->pin);
	_InterlockedOr16 (&latch->pin, lvl);
#endif
	bt_spinreleasewrite(bt->table[hashidx].latch);
	return latch;
  }

	//  see if there are any unused pool entries
#ifdef unix
	slot = __sync_fetch_and_add (&bt->latchmgr->latchdeployed, 1) + 1;
#else
	slot = _InterlockedIncrement (&bt->latchmgr->latchdeployed);
#endif

	if( slot < bt->latchmgr->latchtotal ) {
		latch = bt->latchsets + slot;
		if( bt_latchlink (bt, hashidx, slot, page_no) )
			return NULL;
		bt_spinreleasewrite (bt->table[hashidx].latch);
		return latch;
	}

#ifdef unix
	__sync_fetch_and_add (&bt->latchmgr->latchdeployed, -1);
#else
	_InterlockedDecrement (&bt->latchmgr->latchdeployed);
#endif
  //  find and reuse previous entry on victim

  while( 1 ) {
#ifdef unix
	slot = __sync_fetch_and_add(&bt->latchmgr->latchvictim, 1);
#else
	slot = _InterlockedIncrement (&bt->latchmgr->latchvictim) - 1;
#endif
	// try to get write lock on hash chain
	//	skip entry if not obtained
	//	or has outstanding pins

	slot %= bt->latchmgr->latchtotal;

	//	on slot wraparound, check census
	//	count and increment safe level

	cnt = bt->latchmgr->cache[bt->latchmgr->safelevel];

	if( !slot ) {
	  if( cnt < bt->latchmgr->latchtotal / 10 )
#ifdef unix
		__sync_fetch_and_add(&bt->latchmgr->safelevel, 1);
#else
		_InterlockedIncrement (&bt->latchmgr->safelevel);
#endif
	  continue;
	}

	latch = bt->latchsets + slot;
	idx = latch->page_no % bt->latchmgr->latchhash;
	lvl = (latch->pin & LVL_mask) >> LVL_shift;

	//	see if we are evicting this level yet
	//	or if we are on same chain as hashidx

	if( idx == hashidx || lvl > bt->latchmgr->safelevel )
		continue;

	if( !bt_spinwritetry (bt->table[idx].latch) )
		continue;

	if( latch->pin & ~LVL_mask ) {
	  if( latch->pin & CLOCK_mask )
#ifdef unix
		__sync_fetch_and_add(&latch->pin, -CLOCK_unit);
#else
		_InterlockedExchangeAdd16 (&latch->pin, -CLOCK_unit);
#endif
	  bt_spinreleasewrite (bt->table[idx].latch);
	  continue;
	}

	//  update permanent page area in btree

	page = (BtPage)((uid)slot * bt->page_size + bt->pagepool);
#ifdef unix
	posix_fadvise (bt->idx, page_no << bt->page_bits, bt->page_size, POSIX_FADV_WILLNEED);
	__sync_fetch_and_add (&bt->latchmgr->cache[page->lvl], -1);
#else
	_InterlockedExchangeAdd(&bt->latchmgr->cache[page->lvl], -1);
#endif
	if( page->dirty )
	  if( bt_writepage (bt, page, latch->page_no) )
		return NULL;

	//  unlink our available slot from its hash chain

	if( latch->prev )
		bt->latchsets[latch->prev].next = latch->next;
	else
		bt->table[idx].slot = latch->next;

	if( latch->next )
		bt->latchsets[latch->next].prev = latch->prev;

	bt_spinreleasewrite (bt->table[idx].latch);

	if( bt_latchlink (bt, hashidx, slot, page_no) )
		return NULL;

	bt_spinreleasewrite (bt->table[hashidx].latch);
	return latch;
  }
}

//	close and release memory

void bt_close (BtDb *bt)
{
#ifdef unix
	munmap (bt->table, bt->latchmgr->nlatchpage * bt->page_size);
	munmap (bt->latchmgr, bt->page_size);
#else
	FlushViewOfFile(bt->latchmgr, 0);
	UnmapViewOfFile(bt->latchmgr);
	CloseHandle(bt->halloc);
#endif
#ifdef unix
	if( bt->mem )
		free (bt->mem);
	close (bt->idx);
	free (bt);
#else
	if( bt->mem)
		VirtualFree (bt->mem, 0, MEM_RELEASE);
	FlushFileBuffers(bt->idx);
	CloseHandle(bt->idx);
	GlobalFree (bt);
#endif
}
//  open/create new btree

//	call with file_name, BT_openmode, bits in page size (e.g. 16),
//		size of mapped page pool (e.g. 8192)

BtDb *bt_open (char *name, uint mode, uint bits, uint nodemax)
{
uint lvl, last;
uint nlatchpage, latchhash;
BtLatchMgr *latchmgr;
off64_t size;
uint amt[1];
BtKey key;
BtDb* bt;
int flag;
int stat;
int flags = O_RDWR;
int xmode = (S_IRWXU | S_IRWXG);
int create = (O_RDWR | O_CREAT);

#ifndef unix
OVERLAPPED ovl[1];
#else
struct flock lock[1];
#endif

	// determine sanity of page size and buffer pool

	if( bits > BT_maxbits )
		bits = BT_maxbits;
	else if( bits < BT_minbits )
		bits = BT_minbits;

	if( mode == BT_ro ) {
		fprintf(stderr, "ReadOnly mode not supported: %s\n", name);
		return NULL;
	}
#ifdef unix
	bt = calloc (1, sizeof(BtDb));

    errno = 0;
    stat = access(name, (R_OK | W_OK));
    if ((stat < 0) && (errno != ENOENT)) {

		fprintf(stderr, "unable to open %s, errno: %s\n", name, strerror(errno));
		return free(bt), NULL;

    }
    if (stat == 0) {

        bt->idx = open((char*)name, flags, xmode);

    } else {

        bt->idx = open((char*)name, create, xmode);

    }

    posix_fadvise( bt->idx, 0, 0, POSIX_FADV_RANDOM);
 
	if( bt->idx == -1 ) {
		fprintf(stderr, "unable to open %s\n", name);
		return free(bt), NULL;
	}
#else
	bt = GlobalAlloc (GMEM_FIXED|GMEM_ZEROINIT, sizeof(BtDb));
	attr = FILE_ATTRIBUTE_NORMAL;
	bt->idx = CreateFile(name, GENERIC_READ| GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, attr, NULL);

	if( bt->idx == INVALID_HANDLE_VALUE ) {
		fprintf(stderr, "unable to open %s\n", name);
		return GlobalFree(bt), NULL;
	}
#endif
#ifdef unix
	memset (lock, 0, sizeof(lock));
	lock->l_len = sizeof(struct BtPage_);
	lock->l_type = F_WRLCK;

	if( fcntl (bt->idx, F_SETLKW, lock) < 0 ) {
		fprintf(stderr, "unable to lock record zero %s\n", name);
		return bt_close (bt), NULL;
	}
#else
	memset (ovl, 0, sizeof(ovl));

	//	use large offsets to
	//	simulate advisory locking

	ovl->OffsetHigh |= 0x80000000;

	if( !LockFileEx (bt->idx, LOCKFILE_EXCLUSIVE_LOCK, 0, sizeof(struct BtPage_), 0L, ovl) ) {
		fprintf(stderr, "unable to lock record zero %s, GetLastError = %d\n", name, GetLastError());
		return bt_close (bt), NULL;
	}
#endif 

#ifdef unix
	latchmgr = valloc (BT_maxpage);
	*amt = 0;

	// read minimum page size to get root info

	if(( size = lseek (bt->idx, 0L, 2) )) {
		if(( pread(bt->idx, latchmgr, BT_minpage, 0) == BT_minpage )) {
			bits = latchmgr->alloc->bits;
		} else {
			fprintf(stderr, "Unable to read page zero\n");
			return free(bt), free(latchmgr), NULL;
		}
	}
#else
	latchmgr = VirtualAlloc(NULL, BT_maxpage, MEM_COMMIT, PAGE_READWRITE);
	size = GetFileSize(bt->idx, amt);

	if( size || *amt ) {
		if( !ReadFile(bt->idx, (char *)latchmgr, BT_minpage, amt, NULL) ) {
			fprintf(stderr, "Unable to read page zero\n");
			return bt_close (bt), NULL;
		} else
			bits = latchmgr->alloc->bits;
	}
#endif

	bt->page_size = 1 << bits;
	bt->page_bits = bits;

	bt->mode = mode;

	if( size || *amt ) {
		nlatchpage = latchmgr->nlatchpage;
		goto btlatch;
	}

	if( nodemax < 16 ) {
		fprintf(stderr, "Buffer pool too small: %d\n", nodemax);
		return bt_close(bt), NULL;
	}

	// initialize an empty b-tree with latch page, root page, page of leaves
	// and page(s) of latches and page pool cache

	memset (latchmgr, 0, 1 << bits);
	latchmgr->alloc->bits = bt->page_bits;

	//  calculate number of latch hash table entries

	nlatchpage = (nodemax/16 * sizeof(BtHashEntry) + bt->page_size - 1) / bt->page_size;
	latchhash = nlatchpage * bt->page_size / sizeof(BtHashEntry);

	nlatchpage += nodemax;		// size of the buffer pool in pages
	nlatchpage += (sizeof(BtLatchSet) * nodemax + bt->page_size - 1)/bt->page_size;

	bt_putid(latchmgr->alloc->right, MIN_lvl+1+nlatchpage);
	latchmgr->nlatchpage = nlatchpage;
	latchmgr->latchtotal = nodemax;
	latchmgr->latchhash = latchhash;

	if( bt_writepage (bt, latchmgr->alloc, 0) ) {
		fprintf (stderr, "Unable to create btree page zero\n");
		return bt_close (bt), NULL;
	}

	memset (latchmgr, 0, 1 << bits);
	latchmgr->alloc->bits = bt->page_bits;

	for( lvl=MIN_lvl; lvl--; ) {
		last = MIN_lvl - lvl;	// page number
		slotptr(latchmgr->alloc, 1)->off = bt->page_size - 3;
		bt_putid(slotptr(latchmgr->alloc, 1)->id, lvl ? last + 1 : 0);
		key = keyptr(latchmgr->alloc, 1);
		key->len = 2;		// create stopper key
		key->key[0] = 0xff;
		key->key[1] = 0xff;

		latchmgr->alloc->min = bt->page_size - 3;
		latchmgr->alloc->lvl = lvl;
		latchmgr->alloc->cnt = 1;
		latchmgr->alloc->act = 1;

		if( bt_writepage (bt, latchmgr->alloc, last) ) {
			fprintf (stderr, "Unable to create btree page %.8x\n", last);
			return bt_close (bt), NULL;
		}
	}

	// clear out buffer pool pages

	memset(latchmgr, 0, bt->page_size);
	last = MIN_lvl + nlatchpage;

	if( bt_writepage (bt, latchmgr->alloc, last) ) {
		fprintf (stderr, "Unable to write buffer pool page %.8x\n", last);
		return bt_close (bt), NULL;
	}
		
#ifdef unix
	free (latchmgr);
#else
	VirtualFree (latchmgr, 0, MEM_RELEASE);
#endif

btlatch:
#ifdef unix
	lock->l_type = F_UNLCK;
	if( fcntl (bt->idx, F_SETLK, lock) < 0 ) {
		fprintf (stderr, "Unable to unlock page zero\n");
		return bt_close (bt), NULL;
	}
#else
	if( !UnlockFileEx (bt->idx, 0, sizeof(struct BtPage_), 0, ovl) ) {
		fprintf (stderr, "Unable to unlock page zero, GetLastError = %d\n", GetLastError());
		return bt_close (bt), NULL;
	}
#endif
#ifdef unix
	flag = PROT_READ | PROT_WRITE;
	bt->latchmgr = mmap (0, bt->page_size, flag, MAP_SHARED, bt->idx, ALLOC_page * bt->page_size);
	if( bt->latchmgr == MAP_FAILED ) {
		fprintf (stderr, "Unable to mmap page zero, errno = %d", errno);
		return bt_close (bt), NULL;
	}
	bt->table = (void *)mmap (0, (uid)nlatchpage * bt->page_size, flag, MAP_SHARED, bt->idx, LATCH_page * bt->page_size);
	if( bt->table == MAP_FAILED ) {
		fprintf (stderr, "Unable to mmap buffer pool, errno = %d", errno);
		return bt_close (bt), NULL;
	}
	madvise (bt->table, (uid)nlatchpage << bt->page_bits, MADV_RANDOM | MADV_WILLNEED);
#else
	flag = PAGE_READWRITE;
	bt->halloc = CreateFileMapping(bt->idx, NULL, flag, 0, ((uid)nlatchpage + LATCH_page) * bt->page_size, NULL);
	if( !bt->halloc ) {
		fprintf (stderr, "Unable to create file mapping for buffer pool mgr, GetLastError = %d\n", GetLastError());
		return bt_close (bt), NULL;
	}

	flag = FILE_MAP_WRITE;
	bt->latchmgr = MapViewOfFile(bt->halloc, flag, 0, 0, ((uid)nlatchpage + LATCH_page) * bt->page_size);
	if( !bt->latchmgr ) {
		fprintf (stderr, "Unable to map buffer pool, GetLastError = %d\n", GetLastError());
		return bt_close (bt), NULL;
	}

	bt->table = (void *)((char *)bt->latchmgr + LATCH_page * bt->page_size);
#endif
	bt->pagepool = (unsigned char *)bt->table + (uid)(nlatchpage - bt->latchmgr->latchtotal) * bt->page_size;
	bt->latchsets = (BtLatchSet *)(bt->pagepool - (uid)bt->latchmgr->latchtotal * sizeof(BtLatchSet));

#ifdef unix
	bt->mem = valloc (2 * bt->page_size);
#else
	bt->mem = VirtualAlloc(NULL, 2 * bt->page_size, MEM_COMMIT, PAGE_READWRITE);
#endif
	bt->frame = (BtPage)bt->mem;
	bt->cursor = (BtPage)(bt->mem + bt->page_size);
	return bt;
}

// place write, read, or parent lock on requested page_no.

void bt_lockpage(BtLock mode, BtLatchSet *latch)
{
	switch( mode ) {
	case BtLockRead:
		ReadLock (latch->readwr);
		break;
	case BtLockWrite:
		WriteLock (latch->readwr);
		break;
	case BtLockAccess:
		ReadLock (latch->access);
		break;
	case BtLockDelete:
		WriteLock (latch->access);
		break;
	case BtLockParent:
		WriteLock (latch->parent);
		break;
	}
}

// remove write, read, or parent lock on requested page

void bt_unlockpage(BtLock mode, BtLatchSet *latch)
{
	switch( mode ) {
	case BtLockRead:
		ReadRelease (latch->readwr);
		break;
	case BtLockWrite:
		WriteRelease (latch->readwr);
		break;
	case BtLockAccess:
		ReadRelease (latch->access);
		break;
	case BtLockDelete:
		WriteRelease (latch->access);
		break;
	case BtLockParent:
		WriteRelease (latch->parent);
		break;
	}
}

//	allocate a new page and write page into it

uid bt_newpage(BtDb *bt, BtPage page)
{
BtLatchSet *latch;
uid new_page;
BtPage temp;

	//	lock allocation page

	bt_spinwritelock(bt->latchmgr->lock);

	// use empty chain first
	// else allocate empty page

	if(( new_page = bt_getid(bt->latchmgr->alloc[1].right) )) {
	  if(( latch = bt_pinlatch (bt, new_page) )) {
	  	temp = bt_mappage (bt, latch);
	  } else {
		return 0;
      }

	  bt_putid(bt->latchmgr->alloc[1].right, bt_getid(temp->right));
	  bt_spinreleasewrite(bt->latchmgr->lock);
	  memcpy (temp, page, bt->page_size);

	  bt_update (bt, temp);
	  bt_unpinlatch (latch);
	  return new_page;
	} else {
	  new_page = bt_getid(bt->latchmgr->alloc->right);
	  bt_putid(bt->latchmgr->alloc->right, new_page+1);
	  bt_spinreleasewrite(bt->latchmgr->lock);

	  if(( bt_writepage (bt, page, new_page) )) {
		return 0;
      }
	}

	bt_update (bt, bt->latchmgr->alloc);
	return new_page;
}

//  compare two keys, returning > 0, = 0, or < 0
//  as the comparison value

int keycmp (BtKey key1, unsigned char *key2, uint len2)
{
uint len1 = key1->len;
int ans;

	if(( ans = memcmp (key1->key, key2, len1 > len2 ? len2 : len1) )) {
		return ans;
    }

	if(( len1 > len2 )) {
		return 1;
    }

	if(( len1 < len2 )) {
		return -1;
    }

	return 0;
}

//  Update current page of btree by
//	flushing mapped area to disk backing of cache pool.
//	mark page as dirty for rewrite to permanent location

void bt_update (BtDb *bt, BtPage page)
{
#ifdef unix
	msync (page, bt->page_size, MS_ASYNC);
#else
//	FlushViewOfFile (page, bt->page_size);
#endif
	page->dirty = 1;
}

//  map the btree cached page onto current page

BtPage bt_mappage (BtDb *bt, BtLatchSet *latch)
{
	return (BtPage)((uid)(latch - bt->latchsets) * bt->page_size + bt->pagepool);
}

//	deallocate a deleted page 
//	place on free chain out of allocator page
//	call with page latched for Writing and Deleting

BTERR bt_freepage(BtDb *bt, uid page_no, BtLatchSet *latch)
{
BtPage page = bt_mappage (bt, latch);

	//	lock allocation page

	bt_spinwritelock (bt->latchmgr->lock);

	//	store chain in second right
	bt_putid(page->right, bt_getid(bt->latchmgr->alloc[1].right));
	bt_putid(bt->latchmgr->alloc[1].right, page_no);

	page->free = 1;
	bt_update(bt, page);

	// unlock released page

	bt_unlockpage (BtLockDelete, latch);
	bt_unlockpage (BtLockWrite, latch);
	bt_unpinlatch (latch);

	// unlock allocation page

	bt_spinreleasewrite (bt->latchmgr->lock);
	bt_update (bt, bt->latchmgr->alloc);
	return 0;
}

//  find slot in page for given key at a given level

int bt_findslot (BtDb *bt, unsigned char *key, uint len)
{
uint diff, higher = bt->page->cnt, low = 1, slot;
uint good = 0;

	//	make stopper key an infinite fence value

	if(( bt_getid (bt->page->right) )) {
		higher++;
	} else {
		good++;
    }

	//	low is the lowest candidate, higher is already
	//	tested as .ge. the given key, loop ends when they meet

	while(( diff = higher - low )) {
		slot = low + ( diff >> 1 );
		if(( keycmp (keyptr(bt->page, slot), key, len) < 0 )) {
			low = slot + 1;
		} else {
			higher = slot, good++;
        }
	}

	//	return zero if key is on right link page

 	return good ? higher : 0;
}

//  find and load page at given level for given key
//	leave page rd or wr locked as requested

int bt_loadpage (BtDb *bt, unsigned char *key, uint len, uint lvl, uint lock)
{
uid page_no = ROOT_page, prevpage = 0;
uint drill = 0xff, slot;
BtLatchSet *prevlatch;
uint mode, prevmode;

  //  start at root of btree and drill down

  do {
	// determine lock mode of drill level
	mode = (lock == BtLockWrite) && (drill == lvl) ? BtLockWrite : BtLockRead; 

	if(( bt->latch = bt_pinlatch(bt, page_no) )) {
		bt->page_no = page_no;
	} else {
		return 0;
    }

 	// obtain access lock using lock chaining

	if(( page_no > ROOT_page )) {
		bt_lockpage(BtLockAccess, bt->latch);
    }

	if(( prevpage )) {
		bt_unlockpage(prevmode, prevlatch);
		bt_unpinlatch(prevlatch);
		prevpage = 0;
	}

 	// obtain read lock using lock chaining

	bt_lockpage(mode, bt->latch);

	if(( page_no > ROOT_page )) {
		bt_unlockpage(BtLockAccess, bt->latch);
    }

	//	map/obtain page contents

	bt->page = bt_mappage (bt, bt->latch);

	// re-read and re-lock root after determining actual level of root

	if(( bt->page->lvl != drill)) {
		if(( bt->page_no != ROOT_page )) {
			return bt->err = BTERR_struct, 0;
        }
			
		drill = bt->page->lvl;

		if(( lock != BtLockRead && drill == lvl )) {
			bt_unlockpage(mode, bt->latch);
			bt_unpinlatch(bt->latch);
			continue;
		}
	}

	prevpage = bt->page_no;
	prevlatch = bt->latch;
	prevmode = mode;

	//  find key on page at this level
	//  and descend to requested level

	if(( !bt->page->kill )) {
	 if(( slot = bt_findslot (bt, key, len) )) {
	  if(( drill == lvl )) {
		return slot;
      }

	  while( slotptr(bt->page, slot)->dead ) {
		if(( slot++ < bt->page->cnt )) {
			continue;
		} else {
			goto slideright;
        }
      }

	  page_no = bt_getid(slotptr(bt->page, slot)->id);
	  drill--;
	  continue;
	 }
    }

	//  or slide right into next page

slideright:
	page_no = bt_getid(bt->page->right);

  } while( page_no );

  // return error on end of right chain

  bt->err = BTERR_eof;
  return 0;	// return error
}

//	a fence key was deleted from a page
//	push new fence value upwards

BTERR bt_fixfence (BtDb *bt, uid page_no, uint lvl)
{
unsigned char leftkey[256], rightkey[256];
BtLatchSet *latch = bt->latch;
BtKey ptr;

	// remove deleted key, the old fence value

	ptr = keyptr(bt->page, bt->page->cnt);
	memcpy(rightkey, ptr, ptr->len + 1);

	memset (slotptr(bt->page, bt->page->cnt--), 0, sizeof(BtSlot));
	bt->page->clean = 1;

	ptr = keyptr(bt->page, bt->page->cnt);
	memcpy(leftkey, ptr, ptr->len + 1);

	bt_update (bt, bt->page);
	bt_lockpage (BtLockParent, latch);
	bt_unlockpage (BtLockWrite, latch);

	//  insert new (now smaller) fence key

	if( bt_insertkey (bt, leftkey+1, *leftkey, lvl + 1, page_no, time(NULL)) )
		return bt->err;

	//  remove old (larger) fence key

	if( bt_deletekey (bt, rightkey+1, *rightkey, lvl + 1) )
		return bt->err;

	bt_unlockpage (BtLockParent, latch);
	bt_unpinlatch (latch);
	return 0;
}

//	root has a single child
//	collapse a level from the btree
//	call with root locked in bt->page

BTERR bt_collapseroot (BtDb *bt, BtPage root)
{
BtLatchSet *latch;
BtPage temp;
uid child;
uint idx;

	// find the child entry
	//	and promote to new root

  do {
	for( idx = 0; idx++ < root->cnt; ) {
	  if( !slotptr(root, idx)->dead ) {
		break;
      }
    }

	child = bt_getid (slotptr(root, idx)->id);
	if(( latch = bt_pinlatch (bt, child) )) {
		temp = bt_mappage (bt, latch);
	} else {
		return bt->err;
    }

	bt_lockpage (BtLockDelete, latch);
	bt_lockpage (BtLockWrite, latch);
	memcpy (root, temp, bt->page_size);

	bt_update (bt, root);

	if(( bt_freepage (bt, child, latch) ))
		return bt->err;

  } while( root->lvl > 1 && root->act == 1 );

  bt_unlockpage (BtLockWrite, bt->latch);
  bt_unpinlatch (bt->latch);
  return 0;
}

//  find and delete key on page by marking delete flag bit
//  when page becomes empty, delete it

BTERR bt_deletekey (BtDb *bt, unsigned char *key, uint len, uint lvl)
{
unsigned char lowerkey[256], higherkey[256];
uint slot, dirty = 0, idx, fence, found;
BtLatchSet *latch, *rlatch;
uid page_no, right;
BtPage temp;
BtKey ptr;

	if(( slot = bt_loadpage (bt, key, len, lvl, BtLockWrite) )) {
		ptr = keyptr(bt->page, slot);
	} else {
		return bt->err;
    }

	// are we deleting a fence slot?

	fence = slot == bt->page->cnt;

	// if key is found delete it, otherwise ignore request

	if(( found = !keycmp (ptr, key, len) )) {
	  if(( found = slotptr(bt->page, slot)->dead == 0 )) {
 		dirty = slotptr(bt->page,slot)->dead = 1;
 		bt->page->clean = 1;
 		bt->page->act--;

		// collapse empty slots

		while(( idx = bt->page->cnt - 1 )) {
		  if( slotptr(bt->page, idx)->dead ) {
			*slotptr(bt->page, idx) = *slotptr(bt->page, idx + 1);
			memset (slotptr(bt->page, bt->page->cnt--), 0, sizeof(BtSlot));
		  } else {
			break;
          }
        }
	  }
    }

	right = bt_getid(bt->page->right);
	page_no = bt->page_no;
	latch = bt->latch;

	if(( !dirty )) {
	  if(( lvl )) {
		return bt_abort (bt, bt->page, page_no, BTERR_notfound);
      }
	  bt_unlockpage(BtLockWrite, latch);
	  bt_unpinlatch (latch);
	  return bt->found = found, 0;
	}

	//	did we delete a fence key in an upper level?

	if(( lvl && bt->page->act && fence )) {
	  if(( bt_fixfence (bt, page_no, lvl) )) {
		return bt->err;
	  }else{
		return bt->found = found, 0;
      }
    }

	//	is this a collapsed root?

	if(( lvl > 1 && page_no == ROOT_page && bt->page->act == 1 )) {
	  if(( bt_collapseroot (bt, bt->page) )) {
		return bt->err;
	  }else{
		return bt->found = found, 0;
      }
    }

	// return if page is not empty

	if(( bt->page->act )) {
	  bt_update(bt, bt->page);
	  bt_unlockpage(BtLockWrite, latch);
	  bt_unpinlatch (latch);
	  return bt->found = found, 0;
	}

	// cache copy of fence key
	//	in order to find parent

	ptr = keyptr(bt->page, bt->page->cnt);
	memcpy(lowerkey, ptr, ptr->len + 1);

	// obtain lock on right page

	if(( rlatch = bt_pinlatch (bt, right) )) {
		temp = bt_mappage (bt, rlatch);
	} else {
		return bt->err;
    }

	bt_lockpage(BtLockWrite, rlatch);

	if(( temp->kill )) {
		bt_abort(bt, temp, right, 0);
		return bt_abort(bt, bt->page, bt->page_no, BTERR_kill);
	}

	// pull contents of next page into current empty page 

	memcpy (bt->page, temp, bt->page_size);

	//	cache copy of key to update

	ptr = keyptr(temp, temp->cnt);
	memcpy(higherkey, ptr, ptr->len + 1);

	//  Mark right page as deleted and point it to left page
	//	until we can post updates at higher level.

	bt_putid(temp->right, page_no);
	temp->kill = 1;

	bt_update(bt, bt->page);
	bt_update(bt, temp);

	bt_lockpage(BtLockParent, latch);
	bt_unlockpage(BtLockWrite, latch);

	bt_lockpage(BtLockParent, rlatch);
	bt_unlockpage(BtLockWrite, rlatch);

	//  redirect higher key directly to consolidated node

	if(( bt_insertkey (bt, higherkey+1, *higherkey, lvl+1, page_no, time(NULL)) ))
		return bt->err;

	//  delete old lower key to consolidated node

	if(( bt_deletekey (bt, lowerkey + 1, *lowerkey, lvl + 1) ))
		return bt->err;

	//  obtain write & delete lock on deleted node
	//	add right block to free chain

	bt_lockpage(BtLockDelete, rlatch);
	bt_lockpage(BtLockWrite, rlatch);
	bt_unlockpage(BtLockParent, rlatch);

	if(( bt_freepage (bt, right, rlatch) ))
		return bt->err;

	bt_unlockpage(BtLockParent, latch);
	bt_unpinlatch(latch);
	return 0;
}

//	find key in leaf level and return row-id

uid bt_findkey (BtDb *bt, unsigned char *key, uint len)
{
uint  slot;
BtKey ptr;
uid id;

	if(( slot = bt_loadpage (bt, key, len, 0, BtLockRead) )) {
		ptr = keyptr(bt->page, slot);
	} else {
		return 0;
    }

	// if key exists, return row-id
	//	otherwise return 0

	if(( ptr->len == len && !memcmp (ptr->key, key, len) )) {
		id = bt_getid(slotptr(bt->page,slot)->id);
	}else{
		id = 0;
    }

	bt_unlockpage (BtLockRead, bt->latch);
	bt_unpinlatch (bt->latch);
	return id;
}

//	check page for space available,
//	clean if necessary and return
//	0 - page needs splitting
//	>0 - go ahead with new slot
 
uint bt_cleanpage(BtDb *bt, uint amt, uint slot)
{
uint nxt = bt->page_size;
BtPage page = bt->page;
uint cnt = 0, idx = 0;
uint max = page->cnt;
uint newslot = slot;
BtKey key;

	if( page->min >= (max+1) * sizeof(BtSlot) + sizeof(*page) + amt + 1 )
		return slot;

	//	skip cleanup if nothing to reclaim

	if( !page->clean )
		return 0;

	memcpy (bt->frame, page, bt->page_size);

	// skip page info and set rest of page to zero

	memset (page+1, 0, bt->page_size - sizeof(*page));
	page->act = 0;

	while( cnt++ < max ) {
		if( cnt == slot )
			newslot = idx + 1;
		// always leave fence key in list
		if( cnt < max && slotptr(bt->frame,cnt)->dead )
			continue;

		// copy key
		key = keyptr(bt->frame, cnt);
		nxt -= key->len + 1;
		memcpy ((unsigned char *)page + nxt, key, key->len + 1);

		// copy slot
		memcpy(slotptr(page, ++idx)->id, slotptr(bt->frame, cnt)->id, BtId);
		if( !(slotptr(page, idx)->dead = slotptr(bt->frame, cnt)->dead) )
			page->act++;
#ifdef USETOD
		slotptr(page, idx)->tod = slotptr(bt->frame, cnt)->tod;
#endif
		slotptr(page, idx)->off = nxt;
	}

	page->min = nxt;
	page->cnt = idx;

	if( page->min >= (max+1) * sizeof(BtSlot) + sizeof(*page) + amt + 1 )
		return newslot;

	return 0;
}

// split the root and raise the height of the btree

BTERR bt_splitroot(BtDb *bt, unsigned char *leftkey, uid page_no2)
{
uint nxt = bt->page_size;
BtPage root = bt->page;
uid right;

	//  Obtain an empty page to use, and copy the current
	//  root contents into it

	if( !(right = bt_newpage(bt, root)) )
		return bt->err;

	// preserve the page info at the bottom
	// and set rest to zero

	memset(root+1, 0, bt->page_size - sizeof(*root));

	// insert first key on newroot page

	nxt -= *leftkey + 1;
	memcpy ((unsigned char *)root + nxt, leftkey, *leftkey + 1);
	bt_putid(slotptr(root, 1)->id, right);
	slotptr(root, 1)->off = nxt;
	
	// insert second key on newroot page
	// and increase the root height

	nxt -= 3;
	((unsigned char *)root)[nxt] = 2;
	((unsigned char *)root)[nxt+1] = 0xff;
	((unsigned char *)root)[nxt+2] = 0xff;
	bt_putid(slotptr(root, 2)->id, page_no2);
	slotptr(root, 2)->off = nxt;

	bt_putid(root->right, 0);
	root->min = nxt;		// reset lowest used offset and key count
	root->cnt = 2;
	root->act = 2;
	root->lvl++;

	// update and release root (bt->page)

	bt_update(bt, root);

	bt_unlockpage(BtLockWrite, bt->latch);
	bt_unpinlatch(bt->latch);
	return 0;
}

//  split already locked full node
//	return unlocked.

BTERR bt_splitpage (BtDb *bt)
{
uint cnt = 0, idx = 0, max, nxt = bt->page_size;
unsigned char fencekey[256], rightkey[256];
uid page_no = bt->page_no, right;
BtLatchSet *latch, *rlatch;
BtPage page = bt->page;
uint lvl = page->lvl;
BtKey key;

	latch = bt->latch;

	//  split higher half of keys to bt->frame
	//	the last key (fence key) might be dead

	memset (bt->frame, 0, bt->page_size);
	max = page->cnt;
	cnt = max / 2;
	idx = 0;

	while( cnt++ < max ) {
		key = keyptr(page, cnt);
		nxt -= key->len + 1;
		memcpy ((unsigned char *)bt->frame + nxt, key, key->len + 1);
		memcpy(slotptr(bt->frame,++idx)->id, slotptr(page,cnt)->id, BtId);
		if( !(slotptr(bt->frame, idx)->dead = slotptr(page, cnt)->dead) )
			bt->frame->act++;
#ifdef USETOD
		slotptr(bt->frame, idx)->tod = slotptr(page, cnt)->tod;
#endif
		slotptr(bt->frame, idx)->off = nxt;
	}

	//	remember fence key for new right page

	memcpy (rightkey, key, key->len + 1);

	bt->frame->bits = bt->page_bits;
	bt->frame->min = nxt;
	bt->frame->cnt = idx;
	bt->frame->lvl = lvl;

	// link right node

	if( page_no > ROOT_page )
		memcpy (bt->frame->right, page->right, BtId);

	//	get new free page and write frame to it.

	if( !(right = bt_newpage(bt, bt->frame)) )
		return bt->err;

	//	update lower keys to continue in old page

	memcpy (bt->frame, page, bt->page_size);
	memset (page+1, 0, bt->page_size - sizeof(*page));
	nxt = bt->page_size;
	page->clean = 0;
	page->act = 0;
	cnt = 0;
	idx = 0;

	//  assemble page of smaller keys
	//	(they're all active keys)

	while( cnt++ < max / 2 ) {
		key = keyptr(bt->frame, cnt);
		nxt -= key->len + 1;
		memcpy ((unsigned char *)page + nxt, key, key->len + 1);
		memcpy(slotptr(page,++idx)->id, slotptr(bt->frame,cnt)->id, BtId);
#ifdef USETOD
		slotptr(page, idx)->tod = slotptr(bt->frame, cnt)->tod;
#endif
		slotptr(page, idx)->off = nxt;
		page->act++;
	}

	// remember fence key for smaller page

	memcpy (fencekey, key, key->len + 1);

	bt_putid(page->right, right);
	page->min = nxt;
	page->cnt = idx;

	// if current page is the root page, split it

	if(( page_no == ROOT_page )) {
		return bt_splitroot (bt, fencekey, right);
    }

	//	lock right page

	if(( rlatch = bt_pinlatch (bt, right) )) {
		bt_lockpage (BtLockParent, rlatch);
	}else{
		return bt->err;
    }

	// update left (containing) node

	bt_update(bt, page);

	bt_lockpage (BtLockParent, latch);
	bt_unlockpage (BtLockWrite, latch);

	// insert new fence for reformulated left block

	if( bt_insertkey (bt, fencekey+1, *fencekey, lvl+1, page_no, time(NULL)) )
		return bt->err;

	//	switch fence for right block of larger keys to new right page

	if( bt_insertkey (bt, rightkey+1, *rightkey, lvl+1, right, time(NULL)) )
		return bt->err;

	bt_unlockpage (BtLockParent, latch);
	bt_unlockpage (BtLockParent, rlatch);

	bt_unpinlatch (rlatch);
	bt_unpinlatch (latch);
	return 0;
}

//  Insert new key into the btree at requested level.
//  Pages are unlocked at exit.

BTERR bt_insertkey (BtDb *bt, unsigned char *key, uint len, uint lvl, uid id, uint tod)
{
uint slot, idx;
BtPage page;
BtKey ptr;

  while( 1 ) {
	if(( slot = bt_loadpage (bt, key, len, lvl, BtLockWrite) )) {
		ptr = keyptr(bt->page, slot);
	}
    else
	{
		if( !bt->err )
			bt->err = BTERR_ovflw;
		return bt->err;
	}

	// if key already exists, update id and return

	page = bt->page;

	if( !keycmp (ptr, key, len) ) {
	  if( slotptr(page, slot)->dead )
		page->act++;
	  slotptr(page, slot)->dead = 0;
#ifdef USETOD
	  slotptr(page, slot)->tod = tod;
#endif
	  bt_putid(slotptr(page,slot)->id, id);
	  bt_update(bt, bt->page);
	  bt_unlockpage(BtLockWrite, bt->latch);
	  bt_unpinlatch (bt->latch);
	  return 0;
	}

	// check if page has enough space

	if(( slot = bt_cleanpage (bt, len, slot) ))
		break;

	if( bt_splitpage (bt) )
		return bt->err;
  }

  // calculate next available slot and copy key into page

  page->min -= len + 1; // reset lowest used offset
  ((unsigned char *)page)[page->min] = len;
  memcpy ((unsigned char *)page + page->min +1, key, len );

  for( idx = slot; idx < page->cnt; idx++ )
	if( slotptr(page, idx)->dead )
		break;

  // now insert key into array before slot
  // preserving the fence slot

  if( idx == page->cnt )
	idx++, page->cnt++;

  page->act++;

  while( idx > slot )
	*slotptr(page, idx) = *slotptr(page, idx -1), idx--;

  bt_putid(slotptr(page,slot)->id, id);
  slotptr(page, slot)->off = page->min;
#ifdef USETOD
  slotptr(page, slot)->tod = tod;
#endif
  slotptr(page, slot)->dead = 0;

  bt_update(bt, bt->page);

  bt_unlockpage(BtLockWrite, bt->latch);
  bt_unpinlatch(bt->latch);
  return 0;
}

//  cache page of keys into cursor and return starting slot for given key

uint bt_startkey (BtDb *bt, unsigned char *key, uint len)
{
uint slot;

	// cache page for retrieval

	if(( slot = bt_loadpage (bt, key, len, 0, BtLockRead) )) {
	  memcpy (bt->cursor, bt->page, bt->page_size);
	}else{
	  return 0;
    }

	bt_unlockpage(BtLockRead, bt->latch);
	bt->cursor_page = bt->page_no;
	bt_unpinlatch (bt->latch);
	return slot;
}

//  return next slot for cursor page
//  or slide cursor right into next page

uint bt_nextkey (BtDb *bt, uint slot)
{
BtLatchSet *latch;
off64_t right;

  do {
	right = bt_getid(bt->cursor->right);

	while( slot++ < bt->cursor->cnt )
	  if( slotptr(bt->cursor,slot)->dead ) {
		continue;
	  } else if( right || (slot < bt->cursor->cnt)) {
		return slot;
	  } else {
		break;
      }

	if( !right )
		break;

	bt->cursor_page = right;

	if(( latch = bt_pinlatch (bt, right) )) {
    	bt_lockpage(BtLockRead, latch);
	}else{
		return 0;
    }

	bt->page = bt_mappage (bt, latch);
	memcpy (bt->cursor, bt->page, bt->page_size);
	bt_unlockpage(BtLockRead, latch);
	bt_unpinlatch (latch);
	slot = 0;
  } while( 1 );

  return bt->err = 0;
}

BtKey bt_key(BtDb *bt, uint slot)
{
	return keyptr(bt->cursor, slot);
}

uid bt_uid(BtDb *bt, uint slot)
{
	return bt_getid(slotptr(bt->cursor,slot)->id);
}

#ifdef USETOD
uint bt_tod(BtDb *bt, uint slot)
{
	return slotptr(bt->cursor,slot)->tod;
}
#endif

