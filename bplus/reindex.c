/********************************************************************/
/*                            REINDEX.C                             */
/*                                                                  */
/* This is a sample program which demonstrates how to reindex a     */
/* data file.  The data file which is used is the one created by    */
/* NAMES.C which creates an online address book using the B-PLUS    */
/* file indexing toolkit.                                           */
/*                                                                  */
/********************************************************************/


#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "cclibs/misc/misc.h"
#include "bplus.h"


typedef struct              /* Here is the address record definition */
  {
     char lastname[16];     /* last name           */
     char firstname[16];    /* first name          */
     char address1[31];     /* first address line  */
     char address2[31];     /* second address line */
     char city[21];         /* the city            */
     char state[3];         /* the state           */
     char zipcode[6];       /* postal zip code     */
     char telephone[14];    /* telephone number    */
  }  ADDRESS;


IX_DESC  nameindex;             /* index file variable  */
FILE     *namefile;             /* data file pointer    */
ADDRESS  person;                /* data record variable */


void openfiles(void);
void closefiles(void);
long makeindex(void);


void openfiles()
  /* The file NAMES.DAT already exists.  It is  opened and a new  */
  /* index file is created.                                       */
  {
    if ((namefile = fopen("names.dat","r")) != NULL)
      make_index("names.idx", &nameindex, 1);   /* create index file */
    else
    {
      printf("\nUnable to open NAMES.DAT\n");
      _exit(1);
    }
  } /* openfiles */


void closefiles()
  /* close all files and exit */
  {
    fclose(namefile);
    close_index(&nameindex);
  } /* closefiles */


long makeindex()
  /* read address records and add key to index file */
  {
    ENTRY ee;
    long num, position, size;
    int  ret;
    size = sizeof(person);                    /* data record size */
    ret = fread(&person,size,1,namefile);     /* ret is number of data  */
                                              /* records that were read */
    num = 0L;                                 /* num = data items read */
    position = 0L;                            /* position in datafile */
    while (ret == 1)
      {
        strcpy(ee.key, person.lastname);      /* key is last name followed */
        strcat(ee.key, person.firstname);     /* first name.  Capitalize   */
        strupr(ee.key);			      /*    and copy to ee.key.    */
        ee.recptr = position;                 /* position in datafile  */
        if (add_key(&ee, &nameindex) != IX_OK)     /* add key to index */
           printf("Error while adding key to index file");
        position += size;                     /* new position in datafile */
        num++;                                /* increment data items read */
        ret = fread(&person,sizeof(person),1,namefile);
      }
      return ( num );
  } /* makeindex */


int main(void)
  /* Here is the main program */
  {
    long num;
    openfiles();
    printf("\n\n     MAKING THE NEW INDEX FILE\n\n");
    num = makeindex();
    closefiles();
    printf("     REINDEXING IS COMPLETE - %ld ITEMS WERE INDEXED\n", num);
    return 0;
  } /* main */

