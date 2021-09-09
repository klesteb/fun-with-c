
/*  bplus.h - data structures and constants  */


#define IX_OK       1
#define IX_FAIL     0
#define LOCK_FAIL  (-1)
#define EOIX       (-2)
#define MAXKEY      100
#define NUM_BUFS    8
#define MAX_LEVELS  8
#define IXB_SIZE    1024
#define IXB_SPACE  (IXB_SIZE - sizeof(int) - sizeof(long) * 2)

typedef long RECPOS;

typedef struct                    /*  entry structure in index        */
  {  RECPOS   idxptr;             /*  points to lower index level     */
     RECPOS   recptr;             /*  points to data record           */
     char     key[MAXKEY+1];      /*  start of record key             */
  }  ENTRY;

typedef struct                    /*  index record format             */
  {  RECPOS   brec;               /*  position in index file          */
                                  /*  or location of next free block  */
     int      bend;               /*  first unused block location     */
     RECPOS   p0;                 /*  points to next level            */
     char     entries[IXB_SPACE]; /*  here are the key entries        */
  }  BLOCK;

typedef struct                    /*  disk file info                  */
  {  RECPOS   ff;                 /*  location of first free block    */
     int      nl;                 /*  number of index levels          */
  }  IX_DISK;

typedef struct                    /*  memory buffer pool of indx blks */
  {  int      dirty;              /*  true if changed                 */
     int      handle;             /*  index file handle               */
     int      count;              /*  number of times referenced      */
     BLOCK    mb;
  }  MEMBLOCK;

typedef struct
  {  MEMBLOCK     cache [ NUM_BUFS ];
  }  IX_BUFFER;

typedef struct                    /*  in-memory index descriptor      */
  {  int      ixfile;
     int      root_dirty;         /*  true if root changed            */
     int      level;              /*  level in btree                  */
     int      duplicate;          /*  no duplicate keys if 0          */
     struct
       {  RECPOS    cblock;       /*  position in index file          */
          int       coffset;      /*  current offset within block     */
       }  pos [ MAX_LEVELS ];
     BLOCK    root;               /*  root index record               */
     IX_DISK  dx;
  }  IX_DESC;

int  open_index(char *,IX_DESC *, int);
int  close_index(IX_DESC *);
int  make_index(char *,IX_DESC *, int);
int  first_key(IX_DESC *);
int  last_key(IX_DESC *);
int  next_key(ENTRY *, IX_DESC *);
int  prev_key(ENTRY *, IX_DESC *);
int  find_key(ENTRY *, IX_DESC *);
int  add_key(ENTRY *, IX_DESC *);
int  locate_key(ENTRY *, IX_DESC *);
int  delete_key(ENTRY *, IX_DESC *);
int  find_exact(ENTRY *, IX_DESC *);
void  buffer_flush(IX_DESC *);

