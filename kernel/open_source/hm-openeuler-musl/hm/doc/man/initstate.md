# initstate(), initstate\_r()
Initialize a pseudo-random number generator

## Synopsis
```c
#include <stdlib.h>

char *initstate(unsigned seed, char *state, size_t size);
int initstate_r(unsigned seed, char *state, size_t size, struct random_data *data);
```
## Arguments
- **seed**
A starting point for the random-number sequence. This lets you restart the sequence at the same point.
&nbsp;
- **state**
The state array that you want to initialize.
&nbsp;
- **size**
The size, in bytes, of the state array; see below.
&nbsp;
- **data**
The pointer contains the state information for initializing the pseudo-random num.

## Library
libc

## Description
The initstate() initializes the given *state* array for future use when generating pseudo-random numbers.
&nbsp;
The initstate\_r() function is the reentrant version of the initstate() function, it initializes the given *state* array as well. In addition, the *data* gets the state information for initializing the pseudo-random num.
&nbsp;
These functions use the *size* argument to determine what type of random-number generator to use, the larger the *state* array, the more random the numbers. Values for the amount of *state* information are 8, 32, 64, 128 and 256 bytes. Other values greater than 8 bytes are rounded down to the nearest one of these values. For values smaller than 8, a simple linear congruential random number generator will be used.

## Returns
The initstate() function shall return a pointer to the previous *state* array, or NULL if an error occurred.
&nbsp;
The initstate\_r() function shall return 0 when successful, or -1 if an error occurred.

## Errors
- **EINVAL**
For initstate\_r, EINVAL will be returned, if input arguments are invalid, such as state or data is NULL.

## Examples
&nbsp;
```c
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

static char state1[32];

int main()
{
	initstate(time(NULL), state1, sizeof(state1));
	setstate(state1);
	printf("%d\n", random());
	return EXIT_SUCCESS;
}
```
## Classification
POSIX 1003.1-2017

## Function Safety
TODO
