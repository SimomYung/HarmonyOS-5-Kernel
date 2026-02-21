# hm\_cache\_flush()
Method to write back the contents of cache to main memory

## Synopsis
```c
#include <hmext/cache.h>

int hm_cache_flush(void *va, size_t size)
```

## Arguments

- **va**
Pointer to the virtual base address of flush operation.
&nbsp;
- **size**
Size of flush operation.
&nbsp;

## Library
libc

## Description

Kernel cache driver/module is used for kernel to send msg from user and call cache flush operation in EL1.

&nbsp;
The hm\_cache\_flush() is used to flush the cache.
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
