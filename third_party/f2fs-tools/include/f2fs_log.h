/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: enhance log records for fsck-tools
 * Create: 2023-11-24
*/
#ifndef __F2FS_LOG_H__
#define __F2FS_LOG_H__

#include "f2fs_dfx_common.h"

#define LOG_ERR                             (-1)
#define LOG_OK                              (0)

extern char *g_logTag[7];
struct LogInfo {
    enum LogType type;
    int slogFd;
    int klogFd;
    int klogLevel;
    long slogOffset;
    int needTruncate;
    char *slogFile;
    char *slogFileBak;
    char *logDir;
};
extern struct LogInfo g_logI;

#define KLOG_ERROR_LEVEL        3
#define KLOG_INFO_LEVEL         6
#define KLOG_DEFAULT_LEVEL      KLOG_INFO_LEVEL
#define LOG_BUF_MAX             512

#define KLOGE(fmt, ...)                    \
    KlogWrite(KLOG_ERROR_LEVEL, "<3> %s: " fmt,    \
           g_logTag[g_logI.type], ##__VA_ARGS__)
#define KLOGI(fmt, ...)                    \
    KlogWrite(KLOG_INFO_LEVEL, "<6> %s: " fmt,    \
           g_logTag[g_logI.type], ##__VA_ARGS__)
#define SLOG(x...) SlogWrite(x)

/*
 * TEMP_FAILURE_RETRY is defined by some, but not all, versions of
 * <unistd.h>. (Alas, it is not as standard as we'd hoped!) So, if it's
 * not already defined, then define it here.
 */
/* Copied from system/core/include/cutils/fs.h */
#ifndef TEMP_FAILURE_RETRY
/* Used to retry syscalls that can return EINTR. */
#define TEMP_FAILURE_RETRY(exp) ({        \
    typeof (exp) _rc;            \
    do {                        \
        _rc = (exp);            \
    } while (_rc == -1 && errno == EINTR);    \
_rc; })
#endif

int  SlogInit(int funcType);
void SlogExit(void);
int  SlogWrite(const char *fmt, ...);
void KlogWrite(int level, const char* fmt, ...);

#endif
