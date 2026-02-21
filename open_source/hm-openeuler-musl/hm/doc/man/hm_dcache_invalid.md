# hm\_dcache\_invalid()
Method to mark cache lines as invalid so that future reads go to main memory

## Synopsis
```c
#include <hmext/cache.h>

int hm_dcache_invalid(void *va, size_t size)
```

## Arguments

- **va**
Pointer to the virtual base address of mark operation.
&nbsp;
- **size**
Size of mark operation.
&nbsp;

## Library
libc

## Description

Kernel cache driver/module is used for kernel to send msg from user and call cache flush operation in EL1.

&nbsp;
The hm\_dcache\_invalid() is used to mark the dcache as invalid.
&nbsp;
This function is only supported on BBU.

## Returns

- **0**
Success.
&nbsp;
- **>0**
errors.

## Errors

- **EINVAL**
Invalid args.

## Examples
N/A

## Classifications
Native Interface

## Function Safety
TODO
