# hm\_mm\_va2pa()
Acquire physical address with virtual address

## Synopsis
```c
#include <hmext/mm.h>

int hm_mm_va2pa(void *va, uint64_t *pa)
```

## Arguments

- **va**
The virtual address.
&nbsp;
- **pa**
Physical address as output.

## Library
libc

## Description
hm_mm_va2pa will acquire the corresponding physical address of based on a virtual address.
&nbsp;
This function is only supported on BBU.

## Returns

- **>0**
The error code.
&nbsp;
- **=0**
Success.

## Errors
- **EINVAL**
Invalid arg.
&nbsp;
- **ENOENT**
The virtual address has been demapped.

## Examples
N/A

## Classifications
Native Interface

## Function Safety
TODO
