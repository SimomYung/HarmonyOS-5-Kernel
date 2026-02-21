# hm\_hpi\_open()
Open hpi module

## Synopsis
```c
#include <hmext/hpi.h>

uint64_t hm_hpi_open(const char *module_name);
```

## Arguments

- **module_name**
hpi module name. To open hpi module, the caller should give "hpi\_module" as the name.

## Library
libc

## Description
The hm\_hpi\_open() is used to open the hpi module and return the global unique id of the module (rref) or 0 when error happens.
&nbsp;
This function is only supported on Hi1210 platform.

## Returns

- **>0**
The unique module reference (rref).
&nbsp;
- **0**
Invalid module or open failed.

## Errors
N/A

## Examples
N/A

## Classifications
Native Interface

## Function Safety
TODO
