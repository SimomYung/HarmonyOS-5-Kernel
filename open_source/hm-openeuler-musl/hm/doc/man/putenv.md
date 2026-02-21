# putenv()
Change or add an environment variable

## Synopsis
```c
#include <stdlib.h>

int putenv(char *string);
```

## Arguments

- **string**
A string of the form "name=value" which indicates an environment variable.

## Library
libc

## Description
The putenv() function changes or adds an environment variable specified by *string*. The format of *string* should be "name=value". If "name" is not an existing environment variable name, then *string* is added to the environment, otherwise, the value of "name" in the environment is changed to "value".
&nbsp;
Note that the string pointed to by *string* will be part of the environment variables list, so changing the string will change the corresponding environment variable.
&nbsp;
The putenv() function also provides an additional function that is if *string* does not include an equal sign as shown below:
&nbsp;
```c
putenv("name");
```
&nbsp;
Then the environment variable of name *name* will be unset, the effect is as if calling unsetenv("name").

## Returns

- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors

- **ENOMEM**
The system lacks enough memory to create a new environment variable.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
