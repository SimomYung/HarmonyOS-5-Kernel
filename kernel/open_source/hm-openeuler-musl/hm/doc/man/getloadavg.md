# getloadavg()
Get system load averages

## Synopsis
```c
#include <stdlib.h>

int getloadavg(double loadavg[], int nelem);
```

## Arguments
- **nelem**
The number of samples to be retrieved.
&nbsp;
- **loadavg**
Store the samples retrieved.

## Library
libc

## Description
The getloadavg() function returns the number of processes in the system run queue averaged over various periods of time.

## Returns
- **-1**
The load average was unobtainable.
&nbsp;
- **otherwise**
The number of samples actually retrieved is returned.

## Errors
N/A

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
