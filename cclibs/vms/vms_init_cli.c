/*
 * Taken from a message by Joe Meadows Jr. on the Decus tape for Spring 89.
 * Also see the source for UAF.
 *
 * "table" is a pointer to the internal CLD
 * "name" is a text string containing the name of the verb
 * note that the case of the verb name is important, should be uppercase
 */

#include <stdio.h>
#include <string.h>
#include <descrip.h>
#include <clidef.h>
#include <climsgdef.h>
#include <cli$routines.h>
#include <lib$routines.h>

#include "vms.h"

/*---------------------------------------------------------------------*/

int vms_init_cli(void *table, char *name) {

  char cmd_line[255];
  int i, j, sts;
  struct dsc$descriptor_s cmd_dsc;
  static $DESCRIPTOR(verb, "$VERB");
  static $DESCRIPTOR(line, "$LINE");

  SET_SDESC(cmd_dsc, cmd_line, sizeof(cmd_line));

  if (!((sts = cli$get_value(&verb, &cmd_dsc)) & 1)) lib$stop(sts);

  if (strncmp(cmd_dsc.dsc$a_pointer, name, cmd_dsc.dsc$w_length) == 0) {

    return(1);    /* the command must have been properly defined! */

  } else {

    /* this code assumes that the verb is shorter than than the foreign
     * image specification, which should be reasonable since it includes
     * device:[directory]filename
     */

    if (!((sts = cli$get_value(&line, &cmd_dsc)) & 1)) lib$stop(sts);

    j = strlen(name);
    for (i = 0; (i < cmd_dsc.dsc$w_length) &&
                (cmd_dsc.dsc$a_pointer[i] != ' ') &&
                (cmd_dsc.dsc$a_pointer[i] != '/'); ++i)
      cmd_dsc.dsc$a_pointer[i] = (i < j) ? name[i] : ' ';

    /* initialize the parse tables */

    return(cli$dcl_parse(&cmd_dsc, table, &lib$get_input, &lib$get_input, 0));

  }

}

