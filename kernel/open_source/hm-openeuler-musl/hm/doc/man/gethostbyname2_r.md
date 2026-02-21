# gethostbyname2_r()
The reentrant version of gethostbyname()

## Synopsis

```c
#include <netdb.h>
#include <sys/socket.h>       /* for AF_INET */

int gethostbyname2_r(const char *name, int af,
		     struct hostent *h, char *buf, size_t buflen,
		     struct hostent **res, int *err)
```

## Arguments

- **name**
  The host name string, such as "www.huawei.com". The parameter *name* is case sensitive, for example localhost and LocalHost are different.
  &nbsp;

- **af**
  Address type, valid address types are AF_INET and AF_INET6.
  &nbsp;

- **h**
  A pointer to hostent structure, which will be filled in on success.
  &nbsp;

- **buf**
  The pointer to a user allocated temporary work buffer.
  &nbsp;

- **buflen**
  The temporary work buffer size.
  &nbsp;

- **res**
  After the call, *result* will point to the *ret* on success. In case of an error or if no entry is found *result* will be NULL. It's used to distinguish whether the value *ret* is pointed to is valid.
  &nbsp;

- **err**
  If this call failed, the error number is stored in *err*.

## Library

libc

## Description

The gethostbyname2_r() function is the reentrant version of gethostbyname(). The caller supplies a hostent structure *h* which will be filled in on success, and a temporary work buffer *buf* of size *buflen*. After the call, *res* will point to the *h* on success. If af is not a valid value, h->h_addr will not be filled and return success. In case of an error or if no entry is found *result* will be NULL. The functions return 0 on success and a nonzero error number on failure. In addition to the errors returned by the non-reentrant versions of these functions, if *buf* is too small, the functions will return ERANGE, and the call should be retried with a larger buffer. The address of a variable in which to store error numbers is passed in *err*.

## Returns

The function returns 0 on success and a nonzero error number on failure.
&nbsp;

The return value can be these values as follows:

- **EAGAIN**
  The parameter *name* is a Non-NULL string and can not find its corresponding host, *err* will be set to TRY_AGAIN.
  &nbsp;

- **ENOENT**
  The specified host is unknown. For example, *name* is "", host does not exist, or addr is AF_INET but the parameter *af* is AF_INET6. *err* will be set to HOST_NOT_FOUND.
  &nbsp;

- **ERANGE**
  If *buflen* is 0, *err* will not be set.

## Errors

The variable *err* can have the following values:

- **TRY_AGAIN**
  A temporary error occurred on an authoritative name server. Try again later.
  &nbsp;

- **HOST_NOT_FOUND**
  The specified host is unknown.
  &nbsp;

- **NO_RECOVERY**
  A nonrecoverable name server error occurred.

## Example

```C
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
        ret = gethostbyname2_r(host, AF_INET, &he, buff, bufsz, &result, &herr);
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
