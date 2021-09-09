/*************************************************************************/
/*                                                                       */
/*                             LISTTREE.C                                */
/*                                                                       */
/*  This program displays each block in an index file.  The root node    */
/*  of the index tree is displayed first.  Then the left most descendant */
/*  block is displayed for each level in the tree.  After the leaf block */
/*  is displayed, the right most descendant from the ancestor block is   */
/*  displayed.  That is, a preordering method of transversing the tree   */
/*  is used.  The complete block is displayed including the data file    */
/*  address, the index file address, and the index key.  The printtree   */
/*  routine is recursive.                                                */
/*                                                                       */
/*************************************************************************/

#include <stdio.h>
#include <string.h>
#include "bplus.h"

/* constant and macros */
#define  NULLREC  (-1L)
#define  ENT_ADR(pb,off)  ((ENTRY*)((char*)((pb)->entries) + off))
#define  ENT_SIZE(pe)     strlen((pe)->key) + 1 + 2 * sizeof(RECPOS)

/* global variables defined in BPLUS.C */
extern  IX_DESC    *pci;
extern  BLOCK      *block_ptr;

IX_DESC ixfile;

void retrieve_block(int, RECPOS);
int  copy_entry(ENTRY *, ENTRY *);

void print_blk(pb)                 /* list each entry in block pb */
   BLOCK *pb;
  {
    int i;
    i = 0;
    printf("%ld  ",pb->brec);
    printf("%d  %ld  ",pb->bend,pb->p0);
    while (i < pb->bend)
      {
        printf(" %ld ",ENT_ADR(pb,i) -> idxptr);
        printf(" %ld ",ENT_ADR(pb,i) -> recptr);
        printf("%s    ",ENT_ADR(pb,i) -> key);
        i = i + ENT_SIZE(ENT_ADR(pb,i));
      }
    printf("\n");
  }

void printtree(num, l, level)             /* print the index tree */
   RECPOS num;                            /* index block address */
   int l;                                 /* indent each level l */
   int level;                             /* level in index file */
  {
    long  address;
    int  end, i, j;
    if (num != NULLREC)
      {
        for (i=1; i<=l; i++)
          printf("    ");
        retrieve_block(level, num);
        print_blk(block_ptr);
        address = block_ptr -> p0;
        printtree(address, l + 1,level + 1);
        retrieve_block(level, num);
        j = 0;
        end = block_ptr -> bend;
        while (j < end)
        {
          address = ENT_ADR(block_ptr,j) -> idxptr;
          printtree(address, l + 1, level + 1);
          retrieve_block(level, num);
          j = j + ENT_SIZE(ENT_ADR(block_ptr,j));
        }
      }
    }

int main(void)
  {
    char name[80];

    printf("\n\n                       Display An Index Tree\n\n");
    printf("     Name of index file: ");
    gets(name);
    open_index(name,&ixfile, 1);
    printtree(0L,1,0);
    close_index(&ixfile);
    return 0;
  }
