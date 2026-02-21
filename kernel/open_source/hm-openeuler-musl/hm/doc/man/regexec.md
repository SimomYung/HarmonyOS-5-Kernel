# regexec()
Used to execute a matching between regex string and string

## Synopsis
```c
#include <sys/types.h>
#include <regex.h>

int regexec(const regex_t *restrict preg, const char *restrict string,
	  size_t nmatch, regmatch_t pmatch[restrict], int eflags);
```

## Arguments

- **preg**
Compiled from regcomp and used to matching string.
&nbsp;
- **string**
A string to be matched.
&nbsp;
- **nmatch**
Used to specify the length of the array corresponding to the *pmatch* parameter.
&nbsp;
- **pmatch**
Used to output the specific position that matches *preg* in the string.
&nbsp;
- **eflags**
Used to specify some options in the regular expression matching process.

## Library
libc

## Description
The regexec means regex-execute function is used to execute a matching between regex string and string.

All regular expression matching must via a compiled pattern buffer. So regexec input argument *preg* must be compiled firstly by regcomp.

## Returns
- **0**
On success, this function returns 0.
&nbsp;

- **Error code**
On failure, this function returns an error code.

## Errors
- **REG_NOMATCH**
      Failed to match regex string.
&nbsp;
- **REG_ESPACE**
      The regex routines ran out of memory.

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
