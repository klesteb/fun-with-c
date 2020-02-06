
#ifndef LBRIO_H
#define LBRIO_H 1

/*
 * Define routine templates for librarian I/O.
 */

typedef struct lbr_control { 		/* equiv ofr FILE ptr */
    void *index; 			/* control index for library. */
    long rfa[2];			/* Current record file address */
    int cur_length;
    char *cur_rec;			/* Pointer to current record */
    int used;				/* # of cur_rec read so far */

    int dir_size;			/* Size of module directory */
    int dir_alloc;			/* Size allocated for names */
    int dir_pos;			/* Current offset */
    char *dir_names;			/* Buffer to hold names. */
} *lbr_index;

/*
 * Return value is always VMS condition code.
 */

int lbr_open ( char *name, char *defname, lbr_index *new );
int lbr_close ( lbr_index lptr );
int lbr_set_module ( lbr_index lptr, char *module );
int lbr_fgets ( char *str, int maxchar, lbr_index lptr );
int lbr_getrec(char *str, int maxchar, lbr_index lptr);
int lbr_read(lbr_index lptr, int maxchar, char *str, int *length);
int lbr_read_directory ( char *name, int maxchar, lbr_index lptr );

#endif

