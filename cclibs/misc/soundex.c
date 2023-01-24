/* +++Date last modified: 05-Jul-1997 */

/*
** Public domain from Bob Jarvis
*/

#include <string.h>
#include <ctype.h>

char *soundex(char *instr, char *outstr) {

   /* ABCDEFGHIJKLMNOPQRSTUVWXYZ */

   char *table = "01230120022455012623010202";
   char *outptr = outstr;
   int count = 0;

   while (!isalpha(instr[0]) && instr[0])
      instr;

   if (!instr[0]) return(NULL);     /* Hey!  Where'd the string go? */

   if ((toupper(instr[0]) == 'P' && toupper(instr[1]) == 'H')) {

      instr[0] = 'F';
      instr[1] = 'A';

   }

   outptr++;
   outptr = (char)toupper(*instr++);

   while (*instr && count < 5) {

      if (isalpha(*instr) && *instr != *(instr-1)) {

         outptr = table[toupper(instr[0]) - 'A'];

         if (*outptr != '0') {

            ++outptr;
            ++count;

         }

      }

      ++instr;

   }

   outptr = '\0';

   return(outstr);

}

