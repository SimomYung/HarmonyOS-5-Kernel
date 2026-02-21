# gethostbyaddr_r()

The reentrant version of gethostbyaddr()

## Synopsis

```c
#include <netdb.h>
#include <sys/socket.h>       /* for AF_INET */
extern int h_errno;

int gethostbyaddr_r(const void *addr, socklen_t len, int type,
                    struct hostent *ret, char *buf, size_t buflen,
                    struct hostent **result, int *h_errnop);
```

## Arguments

- **addr**
  The host address you want to query.
  &nbsp;
- **len**
  Length of \**addr*.
  &nbsp;
- **type**
  Address type, valid address types are AF_INET and AF_INET6.
  &nbsp;
- **ret**
  A pointer to hostent structure, which will be filled in on success.
  &nbsp;
- **buf**
  The pointer to a user allocated temporary work buffer.
  &nbsp;
- **buflen**
  The temporary work buffer size.
  &nbsp;

- **result**
  After the call, *result* will point to the *ret* on success. In case of an error or if no entry is found *result* will be NULL. It's used to distinguish whether the value ret is pointed to is valid.
  &nbsp;

- **h_errnop**
  If this call failed, the address of the global variable *h_errno* in which to store error numbers is passed in *h_errnop*.

## Library

libc

## Description

The gethostbyaddr_r() function is the reentrant version of gethostbyaddr(). The caller supplies a hostent structure *ret* which will be filled in on success, and a temporary work buffer *buf* of size *buflen*. After the call, *result* will point to the *ret* on success. In case of an error *result* will be NULL. If no entry is found, *result* will be set to a value whose member h_name is *addr* and return success. The functions return 0 on success and a nonzero error number on failure. In addition to the errors returned by the non-reentrant versions of these functions, if *buf* is too small, the functions will return ERANGE, and the call should be retried with a larger buffer. The global variable *h_errno* is not modified, but the address of a variable in which to store error numbers is passed in *h_errnop*.

## Returns

The functions return 0 on success and a nonzero error number on failure.
&nbsp;

The return value can be these values as follows:

- **EINVAL**
  If parameter *len* or *type* is invalid, *h_errnop* will be set to NO_RECOVERY.
  &nbsp;
- **ERANGE**
  If *buflen* is too small, *h_errnop* will not be set.

## Errors
  The variable *h_errnop* can have the following values:

- **NO_RECOVERY**
  A nonrecoverable name server error occurred.

## Example

```c
int host2addr(const char *host, struct in_addr *addr) {
    struct hostent he, *result;
    int herr, ret, bufsz = 512;
    char *buff = NULL;

    do {
        char *new_buff = (char *)realloc(buff, bufsz);
        if (new_buff == NULL) {
            free(buff);
            return ENOMEM;
        }
        buff = new_buff;
        ret = gethostbyname_r(host, &he, buff, bufsz, &result, &herr);
        bufsz *= 2;
    } while (ret == ERANGE);

    if (ret == 0 && result != NULL)
        *addr = *(struct in_addr *)he.h_addr;
    else if (result != &he)
        ret = herr;

    free(buff);
    return ret;
}
```

## Classification

POSIX 1003.1-2017

## Function Safety

TODO
