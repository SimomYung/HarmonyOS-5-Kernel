# hm\_proc\_check\_regs()
Check whether a register is in specified address range

## Synopsis
```c
#include <hmext/proc.h>

int hm_proc_check_regs(unsigned int which, unsigned long addr, size_t size)
```

## Arguments
- **which**
Which register to check.
&nbsp;
- **addr**
The start address of the check address range.
&nbsp;
- **size**
The size of the check address range.

## Library
libc

## Description
The hm_proc_check_regs() function is used to check whether the *which* register of one of the threads of the calling process is in specified address range. The following registers are supported:
- **HM_PROC_CHECK_REG_PC**
The PC register.
&nbsp;
- **HM_PROC_CHECK_REG_SP**
The SP register.

This function is only supported on Hi1210 platform.

## Returns
- **0**
*which* register is not in the check address range.
&nbsp;
- **1**
*which* register is in the check address range.
&nbsp;
- **-1**
An error occurred.

## Errors
- **EINVAL**
*which* is not supported.

## Examples
&nbsp;

## Classifications
Native Interface

## Function Safety
TODO
