# rand(), rand\_r()
Generate a pseudo-random integer

## Synopsis
```c
#include <stdlib.h>

int rand(void);
int rand_r(unsigned *seed);
```

## Arguments

- **seed**
The pointer to the seed for reentrant generator of pseudo-random integers.

## Library
libc

## Description
The rand() function generates a pseudo-random integer by the seed. The seed can be set by calling the srand() function. The default value of the seed is 1.

The rand\_r() function is the reentrant version of the rand() function, it generates a pseudo-random integer as well. The argument seed is a pointer to an unsigned int which stores the seed between calls. If called with the same value in the argument, the rand\_r() function will generate the same pseudo-random integer.


## Returns

The rand() and rand\_r() shall return a pseudo-random integer range between 0 and RAND\_MAX.

## Errors
N/A

## Examples
```c
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int i;
	unsigned int seed = 0x1234;

	srand(seed);
	for (i = 0; i < 10; ++i) {
		int randomNum = rand();
		printf("The %d num is %d from rand\n", i, randomNum);
	}

	for (i = 0; i < 10; ++i) {
		unsigned int seed_r = seed;
		long randomNum = rand_r(seed_r);
		printf("The %d num is %d from rand_r\n", i, randomNum);
	}
	return 0;
}
```

## Classifications
POSIX 1003.1-2017

## Function Safety
TODO
