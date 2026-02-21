# hm\_uirq\_set\_server()
Set the server of irq

## Synopsis
```c
#include <hmext/irq.h>

int hm_uirq_set_server(unsigned int irq, const uint64_t *new_server, const uint64_t *old_server);
```

## Arguments

- **irq**
The hw_irq number of interrupt.

- **new\_server**
The new server which irq need to set.

- **old\_server**
The old server which irq need to reject.

## Library
libc

## Description
The hm\_uirq\_set\_server() is used to change the server of irq.
&nbsp;
This function is only supported on BBU.

## Returns

- **0**
Success.
&nbsp;
- **<0**
The error codes.

## Errors
N/A

## Examples
N/A

## Classifications
Native Interface

## Function Safety
TODO
