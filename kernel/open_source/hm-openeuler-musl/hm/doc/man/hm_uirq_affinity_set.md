# hm\_uirq\_affinity\_set()
Set the affinity of interrupt

## Synopsis
```c
#include <hmext/irq.h>

int hm_uirq_affinity_set(unsigned int irq, unsigned int cpumask);
```

## Arguments

- **irq**
The hw_irq number of interrupt.
- **cpumask**
The cpumask of the affinity.

## Library
libc

## Description
The hm\_uirq\_affinity\_set() is used to set the cpu affinity of the interrupt.
This function is unavailable with percpu interrupt and it will return INVAL.
&nbsp;
This function is only supported on BBU.

## Returns

- **0**
Success.
&nbsp;
- **<0**
errors.

## Errors

- **EINVAL**
Invalid argument.

## Examples
N/A

## Classifications
Native Interface

## Function Safety
TODO
