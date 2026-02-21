# fpclassify()
Classify the real-floating type

## Synopsis
```c
#include <math.h>

int fpclassify(real-floating x);
```

## Arguments 
- **x**
The real-floating number shall be classified.

## Library
libc

## Description
The fpclassify() classifies the argument *x* as NaN, infinite, normal, subnormal, zero, or into another implementation-defined category. Firstly, an argument represented in a format wider than its semantic type is converted to its semantic type. Then classification is based on the type of the argument.

## Returns
The fpclassify() macro shall return the value of the number classification macro appropriate to the value of its argument.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO

