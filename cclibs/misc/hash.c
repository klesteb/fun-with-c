
#include <string.h>
#include <ctype.h>

unsigned long elf_hash(const unsigned char *buffer) {
/*
 * Function: elf_hash.c
 * Version : 1.0
 * Created : 02-Dec-1994
 * Author  : Kevin Esteb
 *
 * Description:
 *
 *    The following hash function was taken from Software Developement
 *    August 1993. General Purpose Hashing, Page 45. by Andrew Binstock.
 *
 *    Create a hash value for "buffer".
 *
 * Modifications:
 *
 * Varaiables:
 */
 
    unsigned long h = 0;
    unsigned long g = 0;

/*
 * Main part of functon.
 */

    while (*buffer) {

        h = (h << 4) + *buffer++;
        if ((g = h & 0xF0000000))
            h ^= g >> 24;

        h &= ~g;

    }

    return (h);

}

/**/

long pal_hash(const unsigned char *string) {
/*
 * Function: pal_hash.c
 * Version : 1.0
 * Created : 02-Dec-1994
 * Author  : Kevin Esteb
 *
 * Description:
 *
 *    Hash a string using a palindrome.
 *
 * Modifications:
 *
 * Varaiables:
 */
 
   int i = 1, e = strlen(string);
   int l = strlen(string);
   int result = 0;

/*
 * Main part of functon.
 */
      
   while (i < l) {
      
      result += (toupper(string[i]) + tolower(string[e]));
      i++;
      e--;
      
   }
   
   return(result);
   
}          

