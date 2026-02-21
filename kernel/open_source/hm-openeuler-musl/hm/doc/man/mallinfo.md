# mallinfo()
Acquire memory allocation information

## Synopsis
```c
#include <malloc.h>

struct mallinfo mallinfo(void);
```

## Arguments
&nbsp;

## Library
libc

## Description
The mallinfo() function collects infomation about memory allocations performed by malloc(), and return the result as a mallinfo structure.

## Returns
The mallinfo structure is defined as follows:
&nbsp;
```c
struct mallinfo {
	size_t arena;    /* total amount of memory allocated */
	size_t ordblks;  /* number of oridinary free blocks */
	size_t smblks;   /* number of fastpath free blocks */
	size_t hblks;    /* number of blocks allocated using mmap */
	size_t hblkhd;   /* number of bytes in blocks allocated using mmap */
	size_t usmblks;  /* this field is unused */
	size_t fsmblks;  /* number of bytes in fastpath free blocks */
	size_t uordblks; /* number of bytes used in blocks */
	size_t fordblks; /* number of bytes freed in blocks */
	size_t keepcost; /* total amount releasable blocks */
};
```
&nbsp;
- **arena**
The total amount of memory allocated on the heap, which means it includes both in-use blocks and blocks on the free list. However, huge blocks(see **hblks** **hblkhd** below) allocated using mmap are not in this area.
&nbsp;
- **ordblks**
The number of oridinary free blocks, and the ordinary means not fastpath.
&nbsp;
- **smblks**
The number of fastpath free blocks.
&nbsp;
- **hblks**
The number of huge (bigger than CHUNK_SIZE - META_DATA_SIZE, See maloc()) blocks, which are not managered by heap structure in malloc().
&nbsp;
- **hblkhd**
The total bytes of huge blocks.
&nbsp;
- **usmblks**
This field is unused.
&nbsp;
- **fsmblks**
The total bytes of fastpath free blocks.
&nbsp;
- **uordblks**
The number of bytes used in all type of blocks.
&nbsp;
- **fordblks**
The number of bytes freed in all type of blocks.
&nbsp;
- **keepcost**
The total amount of blocks which are releasable.
&nbsp;
## Errors
&nbsp;

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
