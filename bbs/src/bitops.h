
#ifndef BITOPS_H
#define BITOPS_H 1

/*
**  Bit set, clear, and test operations
**
**  based on a public domain snippet by Bob Stout 
**  (from the snippets collection - www.snippets.org)
** 
**  value format = (1L<<1)
*/

#define LBOOL(x) (!(!(x)))

#define bit_set(arg,value) ((arg) | (value))
#define bit_clear(arg,value) ((arg) & ~(value))
#define bit_test(arg,value) LBOOL((arg) & (value))
#define bit_flip(arg,value) ((arg) ^ (value))

#endif

