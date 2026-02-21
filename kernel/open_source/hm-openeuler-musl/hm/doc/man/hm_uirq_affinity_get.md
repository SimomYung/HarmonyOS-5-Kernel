# hm\_uirq\_affinity\_get()
Get the affinity of interrupt

## Synopsis
```c
#include <hmext/irq.h>

int hm_uirq_affinity_get(unsigned int irq, unsigned int *cpumask);
```

## Arguments

- **irq**
The hw_irq number of interrupt.
- **cpumask**
The buf for the cpumask of affinity.

## Library
libc

## Description
The hm\_uirq\_affinity\_get() is used to get the cpu affinity of the interrupt. It will return the cpumask.
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
