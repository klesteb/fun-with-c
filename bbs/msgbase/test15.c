
#include <stdio.h>

typedef struct {           /* msbin float structure                      */
   char m1;
   char m2;
   char m3;
   char exp;
} msbin;

void MbfToLong(msbin *msbinnum, long *num) {
/*
 * File: MbfToLong.c
 * Date: 01-Oct-1992
 * By  : Kevin Esteb
 *
 * Converts 4 byte Microsoft MKS format to long integer.
 * Orginal author unknown.
 *
 */

    unsigned char m1 = msbinnum->m1;
    unsigned char m2 = msbinnum->m2;
    unsigned char m3 = msbinnum->m3;
    unsigned char exp = msbinnum->exp;

    *num = (((m1 + ((unsigned long) m2 << 8) + 
            ((unsigned long) m3 << 16)) | 0x800000L) >> 
            (24 - (exp - 0x80)));

}

void LongToMbf(long num, msbin *msbinnum) {
/*
 * File: LongToMbf.c
 * Date: 04-Oct-1992
 * By  : Kevin Esteb
 *
 * Converts long integer to 4 byte Microsoft MKS format.
 * Orginal code by Bill Werth (1:343/87)
 *
 * Modifications:
 *
 *    Modified the order of equates for the msbin number to work on Amiga.
 *    04-Oct-1992 K.Esteb
 *
 */

   union {
      unsigned char byte[4];
      long gobble;
   } n;

   msbinnum->exp = 0;

   /* normalize mantissa */

   while ((num & 0x800000L) == 0) {

      num <<= 1;
      msbinnum->exp++;

   }

   msbinnum->exp = 24 - msbinnum->exp + 0x80;

   num &= 0x7fffffL;    /* clear sign bit and msb of mantissa */

   n.gobble = num;

#if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
   msbinnum->m1 = n.byte[0];
   msbinnum->m2 = n.byte[1];
   msbinnum->m3 = n.byte[2];
#else
   msbinnum->m1 = n.byte[0];
   msbinnum->m2 = n.byte[2];
   msbinnum->m3 = n.byte[1];
#endif

}

int main(void) {

    msbin junk;
    long num = 1256001;
    long result = 0;

    LongToMbf(num, &junk);
    MbfToLong(&junk, &result);

    printf("number: %ld\n", num);
    printf("result: %ld\n", result);

    return 0;

}

