# hm\_dcache\_clean()
Method to mark cache lines as clean

## Synopsis
```c
#include <hmext/cache.h>

int hm_dcache_clean(void *va, size_t size)
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
The hm\_dcache\_clean() is used to mark the dcache as clean.
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
clean args.

## Examples
N/A

## Classifications
Native Interface

## Function Safety
TODO
