# hm\_uirq\_trigger\_get()
Get the trigger type of interrupt

## Synopsis
```c
#include <hmext/irq.h>

int hm_uirq_trigger_get(unsigned int irq, unsigned int trigger_type);
```

## Arguments

- **irq**
The hw_irq number of interrupt.
- **trigger\_type**
The buf for the trigger type of interrupt.

## Library
libc

## Description
The hm\_uirq\_trigger\_get() is used to get the trigger type of the interrupt.
The trigger type of each CPU should be the same.
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
