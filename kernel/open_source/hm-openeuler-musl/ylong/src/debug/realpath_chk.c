/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: __realpath_chk
 */
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

/* same as glibc */
char *__realpath_chk(const char *buf, char *resolved, size_t resolvedlen)
{
#ifdef PATH_MAX
    if (resolved != NULL && resolvedlen < PATH_MAX) {
        printf("*** buffer overflow detected ***: terminated\n");
        abort();
    }

    return realpath(buf, resolved);
#else
    long int pathmax = pathconf(buf, _PC_PATH_MAX);
    if (pathmax != -1) {
        if (resolvedlen < pathmax) {
            printf("*** buffer overflow detected ***: terminated\n");
            abort();
        }
        return realpath(buf, resolved);
    }

    char *res = realpath(buf, NULL);
    if (res == NULL) {
        return NULL;
    }

    if ((strlen(res) + 1) > resolvedlen) {
        printf("*** buffer overflow detected ***: terminated\n");
        abort();
    }

    (void)strcpy(resolved, res);
    free(res);

    return resolved;
#endif
}
