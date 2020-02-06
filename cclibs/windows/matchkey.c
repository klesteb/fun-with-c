/*------------------------------------------------------------
 * 
 *  matchkey.c
 * 
 *  copyright (c) 1988,89,90 J. Alan Eldridge
 *
 *  since keys are ints, we can't use strchr() to
 *  see if a key is in an array of keys
 * 
 *  the array is terminated with K_ILLEGAL
 * 
 *----------------------------------------------------------*/

#include "windows.h"

int kb_matchkey(int *arr, int val) {

    int n;

    for (n = 0; *arr != KEY_ILLEGAL; arr++, n++)
        if (*arr == val)
            return n;

    return -1;

}

