/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: execinfo header
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _GNU_SOURCE
#include <dlfcn.h>
#include <backtrace_arch.h>

weak_alias(backtrace_aarch, backtrace);

#define ADDR_WIDTH 16

char **backtrace_symbols(void * const * buffer, int size)
{
    if (buffer == NULL || size == 0) {
        return NULL;
    }

    int total = 0;
    int cnt;
    int ret[size];
    Dl_info info[size];
    const char *baseFname;

    for (cnt = 0; cnt < size; cnt++) {
        ret[cnt] = dladdr(buffer[cnt], &info[cnt]);
        if (ret[cnt] == 0) {
            total += 4 + 2 + ADDR_WIDTH + 1; // 4 for <>(), 2 for 0x
            continue;
        }

        baseFname = strrchr(info[cnt].dli_fname, '/');
        if (baseFname == NULL) {
            baseFname = info[cnt].dli_fname;
        } else {
            baseFname++;
        }

        // format <filename + offset>(sysmbolname+offset) 3 for <+> and (+)
        if (baseFname != NULL && baseFname[0] != '\0') {
            total += strlen(baseFname) + 3 + ADDR_WIDTH;
        } else { // format <>(sysmbolname+offset) 2 for <>, 3 for (+)
            total += 2;
        }

        if (info[cnt].dli_sname != NULL && info[cnt].dli_sname[0] != '\0') {
            total += strlen(info[cnt].dli_sname) + 3 + ADDR_WIDTH;
        } else { // format <>(sysmbolname+offset) 2 for <>, 3 for (+)
            total += 3 + ADDR_WIDTH;
        }

        total++; // '\0'
    }

    char **sysmbols = (char **)malloc(sizeof(char *) * size + total);
    if (sysmbols == NULL) {
        return NULL;
    }
    memset(sysmbols, 0, sizeof(char *) * size + total);

    char *last = (char *)(sysmbols + size);
    for (cnt = 0; cnt < size; cnt++) {
        sysmbols[cnt] = last;

        if (ret[cnt] == 0) {
            last += sprintf(last, "<>[%p]", buffer[cnt]);
            last[0] = '\0';
            last++;
            continue;
        }

        baseFname = strrchr(info[cnt].dli_fname, '/');
        if (baseFname == NULL) {
            baseFname = info[cnt].dli_fname;
        } else {
            baseFname++;
        }

        if (baseFname != NULL && baseFname[0] != '\0') {
            last += sprintf(last, "<%s+%p>", baseFname, info[cnt].dli_fbase);
        } else {
            last += sprintf(last, "<>");
        }

        if (info[cnt].dli_sname != NULL && info[cnt].dli_sname[0] != '\0') {
            last += sprintf(last, "(%s+%p)", info[cnt].dli_sname, (char *)buffer[cnt] - (char *)info[cnt].dli_saddr);
        } else {
            last += sprintf(last, "(+%p)", (char *)buffer[cnt] - (char *)info[cnt].dli_fbase);
        }

        last[0] = '\0';
        last++;
    }

    return sysmbols;
}
