# hm\_hpi\_close()
Close hpi module

## Synopsis
```c
#include <hmext/hpi.h>

int hm_hpi_close(uint64_t module_rref);
```

## Arguments

- **module\_rref**
The unique reference of the hpi module.

## Library
libc

## Description
The hm\_hpi\_close() is used to close the hpi module and release the related resources.
&nbsp;
This function is only supported on Hi1210 platform.

## Returns

- **0**
Success.
&nbsp;
- **>0**
errors.

## Errors

- **EINVAL**
Invalid module reference.

## Examples
N/A

## Classifications
Native Interface

## Function Safety
TODO
