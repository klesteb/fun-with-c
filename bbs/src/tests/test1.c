
#include <stdio.h>
#include "mbf.h"

int main(void) {

    msbin junk;
    long result = 0;
    long num = 1256001;

    LongToMbf(num, &junk);
    MbfToLong(&junk, &result);

    printf("number: %ld\n", num);
    printf("result: %ld\n", result);

    return 0;

}

