#include <stdio.h>
#include "putc.h"

int putchar(int c)
{
	return printf("%c", c);
}
