# hm\_uirq\_register()
Regist the irq

## Synopsis
```c
#include <hmext/irq.h>

int hm_uirq_register(unsigned int irq, const uint64_t *server);
```

## Arguments

- **irq**
The hw_irq number of interrupt.
- **server**
The server of interrupt.

## Library
libc

## Description
The hm\_uirq\_register() is used to regist the interrupt.
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
Invalid module reference.

## Examples
N/A

## Classifications
Native Interface

## Function Safety
TODO
