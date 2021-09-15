#ifdef unix
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <sys/time.h>
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

#define slotptr(page, slot) (((BtSlot *)(page+1)) + (slot-1))
#define keyptr(page, slot) ((BtKey)((unsigned char*)(page) + slotptr(page, slot)->off))

extern uid bt_getid(unsigned char *src);
extern void bt_unpinlatch (BtLatchSet *latch);
extern BtLatchSet *bt_pinlatch (BtDb *bt, uid page_no);
extern BtPage bt_mappage (BtDb *bt, BtLatchSet *latch);
extern BTERR bt_readpage (BtDb *bt, BtPage page, uid page_no);
extern BTERR bt_writepage (BtDb *bt, BtPage page, uid page_no);
extern int keycmp (BtKey key1, unsigned char *key2, uint len2);


uint bt_audit (BtDb *bt)
{
uint idx, hashidx;
uid next, page_no;
BtLatchSet *latch;
uint blks[64];
uint cnt = 0;
BtPage page;
BtKey ptr;

#ifdef unix
	posix_fadvise( bt->idx, 0, 0, POSIX_FADV_SEQUENTIAL);
#endif
	if( *(ushort *)(bt->latchmgr->lock) )
		fprintf(stderr, "Alloc page locked\n");
	*(ushort *)(bt->latchmgr->lock) = 0;

	memset (blks, 0, sizeof(blks));

	for( idx = 1; idx <= bt->latchmgr->latchdeployed; idx++ ) {
		latch = bt->latchsets + idx;
		if( *(ushort *)latch->readwr )
			fprintf(stderr, "latchset %d rwlocked for page %.8x\n", idx, (unsigned int)latch->page_no);
		*(ushort *)latch->readwr = 0;

		if( *(ushort *)latch->access )
			fprintf(stderr, "latchset %d accesslocked for page %.8x\n", idx, (unsigned int)latch->page_no);
		*(ushort *)latch->access = 0;

		if( *(ushort *)latch->parent )
			fprintf(stderr, "latchset %d parentlocked for page %.8x\n", idx, (unsigned int)latch->page_no);
		*(ushort *)latch->parent = 0;

		if( latch->pin & PIN_mask ) {
			fprintf(stderr, "latchset %d pinned for page %.8x\n", idx, (unsigned int)latch->page_no);
			latch->pin = 0;
		}
		page = (BtPage)((uid)idx * bt->page_size + bt->pagepool);
		blks[page->lvl]++;

	    if( page->dirty )
	  	 if( bt_writepage (bt, page, latch->page_no) )
		  	fprintf(stderr, "Page %.8x Write Error\n", (unsigned int)latch->page_no);
	}

	for( idx = 0; blks[idx]; idx++ )
		fprintf(stderr, "cache: %d lvl %d blocks\n", blks[idx], idx);

	for( hashidx = 0; hashidx < bt->latchmgr->latchhash; hashidx++ ) {
	  if( *(ushort *)(bt->table[hashidx].latch) )
			fprintf(stderr, "hash entry %d locked\n", hashidx);

	  *(ushort *)(bt->table[hashidx].latch) = 0;
	}

	memset (blks, 0, sizeof(blks));

	next = bt->latchmgr->nlatchpage + LATCH_page;
	page_no = LEAF_page;

	while( page_no < bt_getid(bt->latchmgr->alloc->right) ) {
		if( bt_readpage (bt, bt->frame, page_no) )
		  fprintf(stderr, "page %.8x unreadable\n", (unsigned int)page_no);
		if( !bt->frame->free ) {
		 for( idx = 0; idx++ < bt->frame->cnt - 1; ) {
		  ptr = keyptr(bt->frame, idx+1);
		  if( keycmp (keyptr(bt->frame, idx), ptr->key, ptr->len) >= 0 )
			fprintf(stderr, "page %.8x idx %.2x out of order\n", (unsigned int)page_no, idx);
		 }
		 if( !bt->frame->lvl )
			cnt += bt->frame->act;
		 blks[bt->frame->lvl]++;
		}

		if( page_no > LEAF_page )
			next = page_no + 1;
		page_no = next;
	}

	for( idx = 0; blks[idx]; idx++ )
		fprintf(stderr, "btree: %d lvl %d blocks\n", blks[idx], idx);

	return cnt - 1;
}

#ifndef unix
double getCpuTime(int type)
{
FILETIME crtime[1];
FILETIME xittime[1];
FILETIME systime[1];
FILETIME usrtime[1];
SYSTEMTIME timeconv[1];
double ans = 0;

	memset (timeconv, 0, sizeof(SYSTEMTIME));

	switch( type ) {
	case 0:
		GetSystemTimeAsFileTime (xittime);
		FileTimeToSystemTime (xittime, timeconv);
		ans = (double)timeconv->wDayOfWeek * 3600 * 24;
		break;
	case 1:
		GetProcessTimes (GetCurrentProcess(), crtime, xittime, systime, usrtime);
		FileTimeToSystemTime (usrtime, timeconv);
		break;
	case 2:
		GetProcessTimes (GetCurrentProcess(), crtime, xittime, systime, usrtime);
		FileTimeToSystemTime (systime, timeconv);
		break;
	}

	ans += (double)timeconv->wHour * 3600;
	ans += (double)timeconv->wMinute * 60;
	ans += (double)timeconv->wSecond;
	ans += (double)timeconv->wMilliseconds / 1000;
	return ans;
}
#else
#include <time.h>
#include <sys/resource.h>

double getCpuTime(int type)
{
struct rusage used[1];
struct timeval tv[1];

	switch( type ) {
	case 0:
		gettimeofday(tv, NULL);
		return (double)tv->tv_sec + (double)tv->tv_usec / 1000000;

	case 1:
		getrusage(RUSAGE_SELF, used);
		return (double)used->ru_utime.tv_sec + (double)used->ru_utime.tv_usec / 1000000;

	case 2:
		getrusage(RUSAGE_SELF, used);
		return (double)used->ru_stime.tv_sec + (double)used->ru_stime.tv_usec / 1000000;
	}

	return 0;
}
#endif

//  standalone program to index file of keys
//  then list them onto std-out

int main (int argc, char **argv)
{
uint slot, line = 0, off = 0, found = 0;
int ch, cnt = 0, bits = 12, idx;
unsigned char key[256];
double done, start;
uid next, page_no;
BtLatchSet *latch;
float elapsed;
time_t tod[1];
uint scan = 0;
uint len = 0;
uint map = 0;
BtPage page;
BtKey ptr;
BtDb *bt;
FILE *in;

#ifdef WIN32
	_setmode (1, _O_BINARY);
#endif
	if( argc < 4 ) {
		fprintf (stderr, "Usage: %s idx_file src_file Read/Write/Scan/Delete/Find/Count [page_bits mapped_pool_pages start_line_number]\n", argv[0]);
		fprintf (stderr, "  page_bits: size of btree page in bits\n");
		fprintf (stderr, "  mapped_pool_pages: number of pages in buffer pool\n");
		exit(0);
	}

	start = getCpuTime(0);
	time(tod);

	if( argc > 4 )
		bits = atoi(argv[4]);

	if( argc > 5 )
		map = atoi(argv[5]);

	if( argc > 6 )
		off = atoi(argv[6]);

	bt = bt_open ((argv[1]), BT_rw, bits, map);

	if( !bt ) {
		fprintf(stderr, "Index Open Error %s\n", argv[1]);
		exit (1);
	}

	switch(argv[3][0]| 0x20)
	{
	case 'p':	// display page
		if(( latch = bt_pinlatch (bt, off) ))
			page = bt_mappage (bt, latch);
		else
			fprintf(stderr, "unable to read page %.8x\n", off);

		write (1, page, bt->page_size);
		break;

	case 'a':	// buffer pool audit
		fprintf(stderr, "started audit for %s\n", argv[1]);
		cnt = bt_audit (bt);
		fprintf(stderr, "finished audit for %s, %d keys\n", argv[1], cnt);
		break;

	case 'w':	// write keys
		fprintf(stderr, "started indexing for %s\n", argv[2]);
		if( argc > 2 && (in = fopen (argv[2], "rb")) ) {
#ifdef unix
		  posix_fadvise( fileno(in), 0, 0, POSIX_FADV_NOREUSE);
#endif
		  while( ch = getc(in), ch != EOF )
			if( ch == '\n' )
			{
			  if( off )
		  		sprintf((char *)key+len, "%.9d", line + off), len += 9;

			  if( bt_insertkey (bt, key, len, 0, ++line, *tod) )
				fprintf(stderr, "Error %d Line: %d\n", bt->err, line), exit(0);
			  len = 0;
			}
			else if( len < 245 )
				key[len++] = ch;
		  }
		fprintf(stderr, "finished adding keys for %s, %d \n", argv[2], line);
		break;

	case 'd':	// delete keys
		fprintf(stderr, "started deleting keys for %s\n", argv[2]);
		if( argc > 2 && (in = fopen (argv[2], "rb")) ) {
#ifdef unix
		  posix_fadvise( fileno(in), 0, 0, POSIX_FADV_NOREUSE);
#endif
		  while( ch = getc(in), ch != EOF )
			if( ch == '\n' )
			{
			  if( off )
		  		sprintf((char *)key+len, "%.9d", line + off), len += 9;
			  line++;
			  if( bt_deletekey (bt, key, len, 0) )
				fprintf(stderr, "Error %d Line: %d\n", bt->err, line), exit(0);
			  len = 0;
			}
			else if( len < 245 )
				key[len++] = ch;
		  }
		fprintf(stderr, "finished deleting keys for %s, %d \n", argv[2], line);
		break;

	case 'f':	// find keys
		fprintf(stderr, "started finding keys for %s\n", argv[2]);
		if( argc > 2 && (in = fopen (argv[2], "rb")) ) {
#ifdef unix
		  posix_fadvise( fileno(in), 0, 0, POSIX_FADV_NOREUSE);
#endif
		  while( ch = getc(in), ch != EOF )
			if( ch == '\n' )
			{
			  if( off )
		  		sprintf((char *)key+len, "%.9d", line + off), len += 9;
			  line++;
			  if( bt_findkey (bt, key, len) )
				found++;
			  else if( bt->err )
				fprintf(stderr, "Error %d Syserr %d Line: %d\n", bt->err, errno, line), exit(0);
			  len = 0;
			}
			else if( len < 245 )
				key[len++] = ch;
		  }
		fprintf(stderr, "finished search of %d keys for %s, found %d\n", line, argv[2], found);
		break;

	case 's':	// scan and print keys
		fprintf(stderr, "started scaning\n");
		cnt = len = key[0] = 0;

		if(( slot = bt_startkey (bt, key, len) ))
		  slot--;
		else
		  fprintf(stderr, "Error %d in StartKey. Syserror: %d\n", bt->err, errno), exit(0);

		while(( slot = bt_nextkey (bt, slot) )) {
			ptr = bt_key(bt, slot);
			fwrite (ptr->key, ptr->len, 1, stdout);
			fputc ('\n', stdout);
			cnt++;
	  	}

		fprintf(stderr, " Total keys read %d\n", cnt - 1);
		break;

	case 'c':	// count keys
	  fprintf(stderr, "started counting\n");
	  cnt = 0;

	  next = bt->latchmgr->nlatchpage + LATCH_page;
	  page_no = LEAF_page;

	  while( page_no < bt_getid(bt->latchmgr->alloc->right) ) {
		if(( latch = bt_pinlatch (bt, page_no) ))
			page = bt_mappage (bt, latch);
		if( !page->free && !page->lvl )
			cnt += page->act;
		if( page_no > LEAF_page )
			next = page_no + 1;
		if( scan )
		 for( idx = 0; idx++ < page->cnt; ) {
		  if( slotptr(page, idx)->dead )
			continue;
		  ptr = keyptr(page, idx);
		  if( idx != page->cnt && bt_getid (page->right) ) {
			fwrite (ptr->key, ptr->len, 1, stdout);
			fputc ('\n', stdout);
		  }
		 }
		bt_unpinlatch (latch);
		page_no = next;
	  }
		
	  cnt--;	// remove stopper key
	  fprintf(stderr, " Total keys read %d\n", cnt);
	  break;
	}

	done = getCpuTime(0);
	elapsed = (float)(done - start);
	fprintf(stderr, " real %dm%.3fs\n", (int)(elapsed/60), elapsed - (int)(elapsed/60)*60);
	elapsed = getCpuTime(1);
	fprintf(stderr, " user %dm%.3fs\n", (int)(elapsed/60), elapsed - (int)(elapsed/60)*60);
	elapsed = getCpuTime(2);
	fprintf(stderr, " sys  %dm%.3fs\n", (int)(elapsed/60), elapsed - (int)(elapsed/60)*60);
	return 0;
}

