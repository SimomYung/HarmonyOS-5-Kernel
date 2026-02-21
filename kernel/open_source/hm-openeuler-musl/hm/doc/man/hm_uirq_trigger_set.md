# hm\_uirq\_trigger\_set()
Set the trigger type of interrupt

## Synopsis
```c
#include <hmext/irq.h>

int hm_uirq_trigger_set(unsigned int irq, unsigned int trigger_type);
```

## Arguments

- **irq**
The hw_irq number of interrupt.
- **trigger\_type**
The trigger type of interrupt.

## Library
libc

## Description
The hm\_uirq\_trigger\_set() is used to set the trigger type of the interrupt.
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
