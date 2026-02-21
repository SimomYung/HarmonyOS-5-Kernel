#undef __WORDSIZE
#ifdef __ILP32__
#define __WORDSIZE 32
#else
#define __WORDSIZE 64
#endif
