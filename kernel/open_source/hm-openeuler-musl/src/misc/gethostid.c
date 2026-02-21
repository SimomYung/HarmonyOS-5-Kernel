#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include "libc.h"

#define HOSTID_FILE_PATH "/etc/hostid"

int sethostid (long int id)
{
    if (libc.secure) {
        errno = EPERM;
        return -1;
    }
    int32_t id32 = (int32_t)id;
    if (id32 != id) {
        errno = EOVERFLOW;
        return -1;
    }
    int fd = creat(HOSTID_FILE_PATH, 0644);
    if (fd == -1) {
        return -1;
    }
    ssize_t len = write(fd, &id32, sizeof(id32));
    (void)close(fd);
    return (unsigned long)len != sizeof(id32) ? -1 : 0;
}

long gethostid(void)
{
    int fd = open(HOSTID_FILE_PATH, O_RDONLY|O_LARGEFILE, 0);
    if (fd == -1) {
        return 0;
    }
    int32_t id32;
    ssize_t len = read(fd, &id32, sizeof(id32));
    (void)close(fd);
    return (unsigned long)len != sizeof(id32) ? 0 : id32;
}
