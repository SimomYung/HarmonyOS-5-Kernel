# regerror()
Explain a regular expression error code

## Synopsis
```c
#include <sys/types.h>
#include <regex.h>

size_t regerror(int e, const regex_t *restrict preg, char *restrict buf, size_t size)
```

## Arguments
- **e**
The value returned by a previous call to regcomp() or regexec().
&nbsp;
- **preg**
A pointer to the regex_t object for the regular expression that you provided to the failed call to regcomp() or regexec().
&nbsp;
- **buf**
A pointer to a buffer where the function can store the explanation.
&nbsp;
- **size**
The size of the buffer that returns error information.

## Library
libc

## Description
The regerror() function provides a string explaining an error code returned by regcomp() or regexec(). The string is copied into *buf* for up to *size* characters.

## Returns
The number of characters copied into the buffer.

## Errors
N/A

## Example

```c
#include <sys/types.h>
#include <regex.h>
#include <stdio.h>

int main(int argc, char ** argv)
{
        if (argc != 3) {
                return -1;
        }

        const char * regex = argv[1];
        const char * text= argv[2];

        regex_t preg;
        int err = 0;
        char err_msg[1024] = {0};
        size_t err_msg_len = 0;

        if ((err = regcomp(&preg, regex, 0)) == 0) {
                if ((err = regexec(&preg, text, 0, NULL, 0)) == 0) {
                        printf("Matched: %s matches %s\n", text, regex);
                        regfree(&preg);
                        return 0;
                }
        }

        err_msg_len = regerror(err, &preg, err_msg, sizeof(err_msg));
        err_msg[err_msg_len] = '\0';
        printf("Err: %s\n", err_msg);

        regfree(&preg);
        return 0;
}
```

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
