/*
 * Routines for reading VMS library modules using C fgets semantics.
 * The lbr_read_directory routine will return index 1 module names over
 * successive calls.
 *
 *  int lbr_open ( char *name, char *defname, lbr_index *new );
 *  int lbr_close ( lbr_index lptr );
 *  int lbr_set_module ( lbr_index lptr, char *module );
 *  int lbr_read ( lbr_index lptr, int maxchr, char *str, int *length );
 *  int lbr_fgets ( char *str, int maxchar, lbr_index lptr );
 *  int lbr_read_directory ( char *name, int maxchar, lbr_index lptr );
 *
 *  Author: David Jones
 *  Date:    7-MAY-1994
 *  Revised: 10-MAY-1994    Ported to AXP, cleanup up bugs.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <descrip.h>
#include <lbrdef.h>
#include <lhidef.h>

#include "lbrio.h"

/*
 * Include file doe not define the system routine entry points.
 */
int lbr$ini_control(), lbr$open(), lbr$open(), lbr$close(), lbr$get_header();
int lbr$lookup_key(), lbr$set_locate(), lbr$get_record(), lbr$get_index();

/**************************************************************************/
/* Open help library for access given name and index.
 *   Input:
 *    name    Char *.  Library name to open.
 *    defname Char *.  Default filespec for open (e.g. SYS$HELP:.HLB).
 *    output:
 *    new    lbr_index *.  Receives pointer to control structure for
 *             library access.
 *    Return value:
 *        Value returned in VMS condition value (odd == success).
 */

int lbr_open(char *name, char *defname, lbr_index *new) {

    int status, func, type;
    lbr_index lptr;
    $DESCRIPTOR(fns, "");
    $DESCRIPTOR(dns, "sys$help:*.hlb");

    /*
     * Allocate control block and zero it.
     */

    lptr = (lbr_index)malloc(sizeof(struct lbr_control));
    *new = lptr;

    if (!lptr ) return 12;
    lptr->index = (void *)0;
    lptr->rfa[0] = lptr->rfa[1] = 0;
    lptr->dir_alloc = 0;

    /*
     * Initialize control index.  Put constants in variables so we
     * can properly pass the arguments by reference.
     */

    func = LBR$C_READ;
    type = LBR$C_TYP_UNK;
    status = lbr$ini_control(&lptr->index, &func, &type);

#ifdef DEBUG
    if ( (status&1)==0) printf("init_control status: %d %d\n", status,lptr->index );
#endif

    if ((status & 1) == 0 ) { 

		free(lptr); 
		return status; 

	}

    /*
     * Open the library.
     */

    fns.dsc$w_length = strlen(name);
    fns.dsc$a_pointer = name;
    dns.dsc$w_length = strlen(defname);
    dns.dsc$a_pointer = defname;
    
    status = lbr$open(&lptr->index, &fns, 0, &dns, 0,0,0);

#ifdef DEBUG
    printf("status of lbr open: %d, index=%d\n", status, lptr->index )
#endif

    /*
     * Use locate mode to read records (high performance).
     */

    if ((status & 1) == 0 ) {

		free(lptr);

    } else {

    	status = lbr$set_locate(&lptr->index);
        if ((status & 1) == 0 ) free(lptr);

    }

    return status;

}

/**************************************************************************/
/* Close library openned by lbr_open.  Free allocated memory.
 */
int lbr_close(lbr_index lptr) {

    int status;
    if (!lptr) return 12;        /* bad pointer */

    status = lbr$close(&lptr->index);
    if (lptr->dir_alloc > 0) free (lptr->dir_names);

    free(lptr);

    return status;

}

/**************************************************************************/
/* Lookup module key in library and save RFA address.  This routine
 * must be called prior to lbr_fgets.
 *
 *   input:
 *     lptr    lbr_index.  Pointer returned by lbr_open.
 *    module    char *.  Module name to read.
 *
 *   Return value:
 *        VMS condition code returned by LBR$LOOKUP_KEY().
 */

int lbr_set_module(lbr_index lptr, char *module) {

    int status, one, lbr$set_index();
    $DESCRIPTOR(key, "");

    lptr->used = 0;
    lptr->cur_length = 0;
    key.dsc$w_length = strlen(module);
    key.dsc$a_pointer = module;

    status = lbr$lookup_key(&lptr->index, &key, &lptr->rfa);
#ifdef DEBUG
    printf("Status of module %s lookup: %d %d\n", module, status, lptr->rfa);
#endif

    return status;

}

/**************************************************************************/
/* Read record from library.  Emulate fgets call, append linefeed to each
 * record.  Note that function value is a VMS status, not a character pointer
 * however.
 *
 *   input:
 *    maxchar    int.  Length of str output array.
 *    lptr    lbr_index.  Pointer returned by lbr_open;
 *   output:
 *    str    Char *.  Buffer to receive record read.
 *   Return value:
 *        VMS condition code returned by LBR$GET_RECORD().
 */

int lbr_fgets(char *str, int maxchar, lbr_index lptr) {

    int status, length;
    struct dsc$descriptor cur_rec;

    length = lptr->cur_length - lptr->used;
    if (length <= 0) {

        /*
         * Get next record.
         */

        status = lbr$get_record(&lptr->index, 0, &cur_rec);
        if ((status & 1) == 0) { 

            *str = 0; 
            lptr->cur_length = 0;
            return status; 

        }

        length = cur_rec.dsc$w_length;
        lptr->cur_length = length + 1;        /* implied line feed */
        lptr->cur_rec = cur_rec.dsc$a_pointer;
        lptr->used = 0;

    } else if (length == 1) {

        /*
         * Only implied linefeed left in buffer.
         */
        if (maxchar > 1) *str++ = '\n'; 
        *str = '\0';
        lptr->used = lptr->cur_length;

        return 1;

    } else {

        /*
         * Set cur_rec.
         */

        cur_rec.dsc$w_length = length - 1;
        cur_rec.dsc$a_pointer = &lptr->cur_rec[lptr->used];
        length = cur_rec.dsc$w_length;

    }

    /*
     * Copy to user buffer.
     */

    if (length >= maxchar) length = maxchar - 1;
    memcpy(str, cur_rec.dsc$a_pointer, length);
    if (length < (maxchar - 1)) str[length++] = '\n'; 
    lptr->used += length;
    str[length++] = 0;

#ifdef DEBUG
    printf("status of get_record: %d, length: %d\n", status, 
    lptr->cur_length);
#endif

    return status;

}

/**************************************************************************/
/* Read record from library.  
 * Note that function value is a VMS status, not a character pointer
 * however.
 *
 *   input:
 *    maxchar    int.  Length of str output array.
 *    lptr    lbr_index.  Pointer returned by lbr_open;
 *   output:
 *    str    Char *.  Buffer to receive record read.
 *   Return value:
 *        VMS condition code returned by LBR$GET_RECORD().
 */

int lbr_getrec(char *str, int maxchar, lbr_index lptr) {

    int status, length;
    struct dsc$descriptor cur_rec;

    length = lptr->cur_length - lptr->used;
    if (length <= 0) {

        /*
         * Get next record.
         */

        status = lbr$get_record(&lptr->index, 0, &cur_rec);
        if ((status & 1) == 0) { 

            *str = 0; 
            lptr->cur_length = 0;
            return status; 

        }

        length = cur_rec.dsc$w_length;
        lptr->cur_length = length;
        lptr->cur_rec = cur_rec.dsc$a_pointer;
        lptr->used = 0;

    } else if (length == 1) {

        /*
         * Only implied linefeed left in buffer.
         */

        *str = '\0';
        lptr->used = lptr->cur_length;

        return 1;

    } else {

        /*
         * Set cur_rec.
         */

        cur_rec.dsc$w_length = length;
        cur_rec.dsc$a_pointer = &lptr->cur_rec[lptr->used];
        length = cur_rec.dsc$w_length;

    }

    /*
     * Copy to user buffer.
     */

    if (length > maxchar) length = maxchar;
    memcpy(str, cur_rec.dsc$a_pointer, length);
    lptr->used += length;
    str[length++] = '\0';

#ifdef DEBUG
    printf("status of get_record: %d, length: %d\n", status, lptr->cur_length);
#endif

    return status;

}

/**************************************************************************/
/* Read record from library.  
 * Note that function value is a VMS status, not a character pointer
 * however.
 *
 *   input:
 *    maxchar    int.  Length of str output array.
 *    lptr    lbr_index.  Pointer returned by lbr_open;
 *   output:
 *    str    Char *.  Buffer to receive record read.
 *   Return value:
 *        VMS condition code returned by LBR$GET_RECORD().
 */

int lbr_read(lbr_index lptr, int maxchar, char *str, int *size) {

    int status, length;
    struct dsc$descriptor cur_rec;

    length = lptr->cur_length - lptr->used;
    if (length <= 0) {

        /*
         * Get next record.
         */

        status = lbr$get_record(&lptr->index, 0, &cur_rec);
        if ((status & 1) == 0) { 

            *str = 0; 
            lptr->cur_length = 0;
            return status; 

        }

        length = cur_rec.dsc$w_length;
        lptr->cur_length = length;
        lptr->cur_rec = cur_rec.dsc$a_pointer;
        lptr->used = 0;

    } else if (length == 1) {

        /*
         * Only implied linefeed left in buffer.
         */

        *str = '\0';
        lptr->used = lptr->cur_length;

        return 1;

    } else {

        /*
         * Set cur_rec.
         */

        cur_rec.dsc$w_length = length;
        cur_rec.dsc$a_pointer = &lptr->cur_rec[lptr->used];
        length = cur_rec.dsc$w_length;

    }

    /*
     * Copy to user buffer.
     */

    if (length > maxchar) length = maxchar;
    memcpy(str, cur_rec.dsc$a_pointer, length);
    lptr->used += length;
    str[length++] = '\0';
	*size = length;

#ifdef DEBUG
    printf("status of get_record: %d, length: %d\n", status, lptr->cur_length);
#endif

    return status;

}

/**************************************************************************/
/* Internal routine for getting library module directory with LBR$GET_INDEX.
 *  A bizarre oversight in LBR$GET_INDEX's design means we have to use
 *  a module-wide variable (load_tmp) to pass context information to this
 *  routine.
 */

static lbr_index load_tmp;
static int lbr_fill_dir(struct dsc$descriptor *name, long *rfa, 
                        lbr_index tptr ) {

    int length;
    lbr_index lptr;

    lptr = load_tmp;
    length = name->dsc$w_length;
    while (length + lptr->dir_size + 1 >= lptr->dir_alloc) {

#ifdef DEBUG
    printf("Reallocating directory\n");
#endif

        lptr->dir_names = realloc(lptr->dir_names, 
                                  (lptr->dir_alloc + length + 1) * 2);
        lptr->dir_alloc *= 2;

    }

    memcpy(&lptr->dir_names[lptr->dir_size], name->dsc$a_pointer, length);
    lptr->dir_names[lptr->dir_size+length] = '\0';
    lptr->dir_size += length + 1;

    return 1;

}

/**************************************************************************/
/* Return next module name in libraries directory.
 *
 *   input:
 *    maxchar    Int.  Size of output name array.
 *    lptr    lbr_index.  Pointer returned by lbr_open().
 *   output:
 *    name    char *.  Module name.
 *   return value:
 *        VMS condition code returned by LBR$GET_INDEX().
 */
int lbr_read_directory(char *name, int maxchar, lbr_index lptr) {

    int status, count, inum, length;

    /*
     * Initialize.
     */

    if ( lptr->dir_alloc <= 0 ) {

        struct lhidef header;
        status = lbr$get_header(&lptr->index, &header);
        count = header.lhi$l_modcnt;

        lptr->dir_alloc = count * 20;
        lptr->dir_size = lptr->dir_pos = 0;
        lptr->dir_names = malloc(lptr->dir_alloc);

        load_tmp = lptr;    /* bogus */
        inum = 1;
        status = lbr$get_index(&lptr->index, &inum, lbr_fill_dir, lptr);

    } else status = 1;

    if (lptr->dir_pos >= lptr->dir_size) return 2160;

    length = strlen( &lptr->dir_names[lptr->dir_pos]);

    if (length < maxchar) {

        strcpy(name, &lptr->dir_names[lptr->dir_pos]);

    } else {

        strncpy(name, &lptr->dir_names[lptr->dir_pos], maxchar-1);
        name[maxchar - 1] = '\0';

    }

#ifdef DEBUG
printf("read dir pos: %d, name: '%s'\n", lptr->dir_pos, name );
#endif

    lptr->dir_pos += length + 1;

    return status;

}

