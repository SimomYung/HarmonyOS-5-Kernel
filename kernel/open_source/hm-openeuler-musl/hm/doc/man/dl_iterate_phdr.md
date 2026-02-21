# dl\_iterate\_phdr()
Walk through all the shared objects loaded in a process

## Synopsis
```c
#define _GNU_SOURCE
#include <dlfcn.h>

int dl_iterate_phdr(int (*callback)(struct dl_phdr_info *info, size_t size, void *data), void *data);
```

## Arguments
- **callback**
The callback function which will walk through all the shared objects.
&nbsp;
- **data**
The parameter passed to callback function.

## Library
libc

## Description
The function dl\_iterate\_phdr() will walk through all the loaded shared objects one by one by loaded order.
&nbsp;
*callback* is the callback iteration function, it takes 3 parameters: *info* is the pointer to shared object information, see below definition. *size* is the structure size. *data* is the parameter passed by dl\_iterate\_phdr.
&nbsp;

```c
struct dl_phdr_info {
	ElfW(Addr) dlpi_addr;              /* Base address of object */
	const char *dlpi_name;             /* Name of object */
	const ElfW(Phdr) *dlpi_phdr;       /* Pointer to array of ELF program headers */
	ElfW(Half) dlpi_phnum;             /* Numbers of program headers */
	unsigned long long int dlpi_adds;  /* Incremented when a new object may have added */
	unsigned long long int dlpi_subs;  /* Incremented when an object may have been removed */
	size_t dlpi_tls_modid;             /* If there is a PT_TLS segment, its module ID as used in TLS relocations, else zero */
	void *dlpi_tls_data;               /* The address of the calling thread's instance of this module's PT_TLS segment, if it has one and it has been allocated in the calling thread, otherwise a null pointer */

};
```

## Returns

- **any**
dl\_iterate\_phdr() returns whatever value was returned by the last call to *callback*.

## Errors
N/A

## Example
&nbsp;

## Classification
Unix

## Function Safety
TODO
