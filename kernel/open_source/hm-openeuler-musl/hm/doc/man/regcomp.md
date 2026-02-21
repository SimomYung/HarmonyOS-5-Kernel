# regcomp()
Used to compile a regular expression into a format buffer

## Synopsis
```c
#include <sys/types.h>
#include <regex.h>

int regcomp(regex_t *restrict preg, const char *restrict regex, int cflags);
```
## Arguments

- **preg**
The preg argument is used to save the compiled regular expression results.
The regmatch_t struct is defined in &lt;regex.h&gt;.
&nbsp;
- **regex**
A string of regular expressions to be compiled.
&nbsp;
- **cflags**
Used to specify some options in the regular expression matching process.

## Library
libc

## Description
The regcomp means regex-compile function is used to compile a regular expression into a form string for regexec.

The argument of *cflags* can be zero or more of the following:
- **REG_EXTENDED**
Enable the POSIX regular library extension. For detailed information about this extension, refer to the POSIX specification. If not set, posix basic regular expression syntax is used. Especially back reference only to basic regular expressions.
&nbsp;
- **REG_NOSUB**
Do not store substrings.
&nbsp;
- **REG_NEWLINE**
Use line breaks as delimiters for multiple lines.

All regular expression matching must via a compiled pattern buffer. So regexec(), regerror(), regfree() input argument *preg* must be compiled firstly by regcomp.

## Returns
- **0**
On success, this function returns 0.
&nbsp;

- **Error code**
On failure, this function returns an error code.

## Errors
- **REG_BADBR**
      Invalid use of back reference operator.
&nbsp;
- **REG_BADPAT**
      Invalid use of pattern operators.
&nbsp;
- **REG_BADRPT**
      Invalid use of repetition operators.
&nbsp;
- **REG_EBRACE**
      Mismatched bracket-spaced operator.
&nbsp;
- **REG_EBRACK**
      Mismatched bracket-list operators.
&nbsp;
- **REG_ECOLLATE**
      Invalid collating element.
&nbsp;
- **REG_ECTYPE**
      Unknown character type.
&nbsp;
- **REG_EESCAPE**
      Trailing backslash.
&nbsp;
- **REG_EPAREN**
      Mismatched parentheses group operator.
&nbsp;
- **REG_ERANGE**
      Invalid use of the range operator.
&nbsp;
- **REG_ESPACE**
      The regex ran out of memory
&nbsp;
- **REG_ESUBREG**
      Invalid back reference to a subexpression.

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
