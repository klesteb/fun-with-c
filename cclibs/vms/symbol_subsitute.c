
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 1999 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ssdef.h>
#include <descrip.h>
#include <str$routines.h>
#include <lib$routines.h>

#include "vms.h"

/*--------------------------------------------------------------------------*/

void symbol_subsitute(struct dsc$descriptor *old_command, 
                      struct dsc$descriptor *new_command) {
/*
 * Function: symbol_subsitute.c
 * Author  : Kevin Esteb
 * Version : 1.0
 * Date    : 21-Jun-1993
 *
 * Synopsis
 *
 *    This function will check for any symbols in the command line. If any
 *    are found they are then retrieved from the symbol table. This function
 *    performs very simple symbol subsitution. 
 *
 *    Example:
 *
 *        program/output='junk_file' 'input_file'
 *
 * Modifications
 *
 * Variables
 */

    int x;
    int stat;
    int done = FALSE;

    long  begin;
    long  table;
    long  start;
    long  finish;
    short length;

    $DESCRIPTOR(sq, "\'");
    $DESCRIPTOR(dq, "\'\'");
    struct dsc$descriptor symbol = {0, DSC$K_DTYPE_T, DSC$K_CLASS_D, 0};
    struct dsc$descriptor temp_1 = {0, DSC$K_DTYPE_T, DSC$K_CLASS_D, 0};
    struct dsc$descriptor temp_2 = {0, DSC$K_DTYPE_T, DSC$K_CLASS_D, 0};
    struct dsc$descriptor dcl_value = {0, DSC$K_DTYPE_T, DSC$K_CLASS_D, 0};

/*
 * Main part of function.
 */ 

    stat = str$copy_dx(new_command, old_command);
    vms_check_status(stat);

    while (!done) {

        if ((start = str$position(new_command, &dq, &1L)) != 0) {

            x = 2;

        } else if ((start = str$position(new_command, &sq, &1L)) != 0) {
            
            x = 1;

        }

        if (start) {

            begin = (start + x);

            if ((finish = str$position(new_command, &sq, &begin)) != 0) {

                start = start + x;
                finish = finish - 1;

                stat = str$pos_extr(&symbol, new_command, &start, &finish);
                vms_check_status(stat);

                stat = lib$get_symbol(&symbol, &dcl_value, &length, &table);
                if (stat == SS$_NORMAL) {

                    start = start - x - 1;
                    finish = finish + 2;

                    stat = str$left(&temp_1, new_command, &start);
                    vms_check_status(stat);

                    stat = str$right(&temp_2, new_command, &finish);
                    vms_check_status(stat);

                    stat = str$concat(new_command, &temp_1, &dcl_value, &temp_2);
                    vms_check_status(stat);

                }

            } else done = TRUE;

        } else done = TRUE;

    }

}

