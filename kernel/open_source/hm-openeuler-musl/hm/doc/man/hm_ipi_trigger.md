# hm\_ipi\_trigger()
Method to generate sgi to send to sre core

## Synopsis
```c
#include <hmext/irq.h>

int hm_ipi_trigger(unsigned int ipi, unsigned int cpumask)
```

## Arguments

- **ipi**
Interrupt ID.
&nbsp;
- **cpumask**
Number of cpu to send.
&nbsp;

## Library
libc

## Description

Kernel irq driver/module is used for sending interrupt from user to those cpus which sre is running.

&nbsp;
The hm\_ipi\_trigger() is used to generate sgi.
&nbsp;
This function is only supported on BBU.

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
