# hm\_uirq\_disable()
Disable the interrupt

## Synopsis
```c
#include <hmext/irq.h>


int hm_uirq_disable(unsigned int irq);
```

## Arguments

- **irq**
The hw_irq number of interrupt.

## Library
libc

## Description
The hm\_uirq\_disable() could disable the interrupt. 
&nbsp;
This function is only supported on BBU.

## Returns

- **0**
Success.
&nbsp;
- **<0**
The error codes.

## Errors
- **EINVAL**
Invalid argument.

## Examples
N/A

## Classifications
Native Interface

## Function Safety
TODO
