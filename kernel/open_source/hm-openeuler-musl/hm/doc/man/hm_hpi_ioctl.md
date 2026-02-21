# hm\_hpi\_ioctl()
The control method to hpi module

## Synopsis
```c
#include <hmext/hpi.h>

int hm_hpi_ioctl(uint64_t module_rref, uint32_t cmd,
                 void *cfg, uint32_t cfg_len,
                 void *rlt, uint32_t rlt_len);
```

## Arguments

- **module\_rref**
The unique reference of the hpi module.
&nbsp;
- **cmd**
Cmd to hpi module. This is synced from products. We should not use self defined cmds.
&nbsp;
- **cfg**
Pointer to the input config buffer.
&nbsp;
- **cfg\_len**
The length of input config.
&nbsp;
- **rlt**
The output result buffer.
&nbsp;
- **rlt\_len**
The length of output result buffer.

## Library
libc

## Description

RRU HPI is a performance sensitive module and has to be put in kernel space. This is the main op interface provided by kernel so that user app could send cmd to hpi module. The cmd and arg are negotiated between the lower-level and caller. The lib is customized for rru.
&nbsp;
The hm\_hpi\_ioctl() is used to close the hpi module and release the related resources.
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
Invalid args.

## Examples
N/A

## Classifications
Native Interface

## Function Safety
TODO
