# The hm\_uirq\_wait()
Wait for interrupt

## Synopsis
```c
#include <hmext/irq.h>

int hm_uirq_wait(void);
```

## Arguments
N/A

## Library
libc

## Description
The hm\_uirq\_wait() is used to wait for the trigger of interrupt.
&nbsp;
This function is only supported on BBU.

## Returns

- **>=0**
The irq number of interrupt.
&nbsp;
- **<0**
errors.

## Errors
N/A

## Examples
N/A

## Classifications
Native Interface

## Function Safety
TODO
