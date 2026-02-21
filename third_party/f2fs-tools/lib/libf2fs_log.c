/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: enhance log records for fsck-tools
 * Create: 2023-11-24
*/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "securec.h"
#include <sys/uio.h>

#include <f2fs_fs.h>
#include <f2fs_log.h>

#define LOG_PARTITION_DEV                   "/dev/"
#define LOG_PARTITION_LOG                   "/log/"
#define LOG_MAX_SIZE                        0x40000 /* 256KB */
#define LOG_FILE_O_FLAGS                    0664
#define LOG_DIR_MODE                        0775
#define LOG_KMSG_NODE_BASE_MODE             0600
#define LOG_PARTITION_SIZE                  2
#define LOG_SLOG_FILENAME_DIFFERENCE        2
#define LOG_SLOG_INIT_MEM_BUF_ELEMENT_NUM   1
#define LOG_SLOG_INIT_MEM_BUF_SIZE          64
#define LOG_BUF_INITIAL_SIZE                64
#define LOG_TM_STARTING_YEAR                1900
#define LOG_KMSG_NODE_DEV                   ((1 << 8) | 11)
#define LOG_BUF_SIZE_PROTECT_THRESHOLD      2048 /* maximum buffer size allowed */
#define UID_ROOT                            0
#define GID_SYSTEM                          1000

char *g_logTag[7] = {
    "F2FS-tools",
    "F2FS.fsck",
    "F2FS.dump",
    "F2FS.defrag",
    "F2FS.resize",
    "F2FS.mkfs",
    "",
};

static char *g_logPartition[LOG_PARTITION_SIZE] = {
    LOG_PARTITION_LOG,
    LOG_PARTITION_DEV
};

struct LogInfo g_logI = {
    .type           = LOG_TYP_NONE,
    .slogFd         = -1,
    .klogFd         = -1,
    .klogLevel      = KLOG_DEFAULT_LEVEL,
    .slogOffset     = 0,
    .needTruncate   = 0,
    .slogFile       = NULL,
    .slogFileBak    = NULL,
    .logDir         = NULL
};

static int DoSlogInit(int logType);
static void KlogInit(void);
static void KlogWritev(int level, const struct iovec* iov, int iovCount);
static int SlogFixSize(int requestSize);
static int InitLogStruct();
static int InitLogFilenames(int logType, char *name);
static int InitLogInfo(int logType);

/*
 * klog_* are copied from system/core/libcutils/klog.c
 * klog write data to /dev/kmsg, to save message in kernel log
 */

static void KlogInit(void)
{
    if (g_logI.klogFd >= 0) {
        return; /* Already initialized */
    }

    g_logI.klogFd = open("/dev/kmsg", O_WRONLY | O_CLOEXEC);
    if (g_logI.klogFd >= 0) {
        return;
    }

    static const char* name = "/dev/__kmsg__";
    if (mknod(name, S_IFCHR | LOG_KMSG_NODE_BASE_MODE, LOG_KMSG_NODE_DEV) == 0) {
        g_logI.klogFd = open(name, O_WRONLY | O_CLOEXEC);
        unlink(name);
    }
}

static void KlogWritev(int level, const struct iovec* iov, int iovCount)
{
    if (level > g_logI.klogLevel) {
        return;
    }
    if (g_logI.klogFd < 0) {
        KlogInit();
    }
    if (g_logI.klogFd < 0) {
        return;
    }
    TEMP_FAILURE_RETRY(writev(g_logI.klogFd, iov, iovCount));
}

void KlogWrite(int level, const char* fmt, ...)
{
    int ret;
    char *buf;
    va_list ap;

    buf = calloc(1, LOG_BUF_MAX);
    if (!buf) {
        KLOGE("calloc klog buf failed\n");
        return;
    }

    va_start(ap, fmt);
    ret = vsnprintf_s(buf, LOG_BUF_MAX, LOG_BUF_MAX - 1, fmt, ap);
    printf("dbg klog : ");
    printf(buf);
    va_end(ap);

    if (ret < 0) {
        printf("klog string is oversized.\n");
    }

    struct iovec iov[1];
    iov[0].iov_base = buf;
    iov[0].iov_len = strlen(buf);
    KlogWritev(level, iov, 1);
    free(buf);
}

static int SlogFixSize(int requestSize)
{
    struct stat st;
    int mode;

    if (stat(g_logI.slogFile, &st) < 0) {
        if (errno == ENOENT) {
            return 0;
        }
        KLOGE("cannot stat %s errno %d\n", g_logI.slogFile, errno);
        return LOG_ERR;
    }

    if (!S_ISREG(st.st_mode)) {
        KLOGE("file %s is not a regular file\n", g_logI.slogFile);
        return LOG_ERR;
    }

    if (LOG_MAX_SIZE - st.st_size > requestSize) {
        return 0;
    }

    /*
     * LOG_FILE does not have enough size, we rename it to LOG_FILE_BAK,
     * then unlink LOG_FILE, so that we can write new log to LOG_FILE
     * again
     */
    close(g_logI.slogFd);
    if (rename(g_logI.slogFile, g_logI.slogFileBak)) {
        KLOGE("rename fail errno %d\n", errno);
        /* force unlink LOG_FILE */
        unlink(g_logI.slogFile);
    }

    KLOGI("%s is full, rename it to %s\n", g_logI.slogFile,
        g_logI.slogFileBak);
    if (access(g_logI.slogFile, F_OK) < 0) {
        if (errno != ENOENT) {
            KLOGE("cannot unlink %s, errno %d\n", g_logI.slogFile,
                errno);
            return LOG_ERR;
        }
    } else {
        KLOGE("fall to rename %s\n", g_logI.slogFile);
        return LOG_ERR;
    }

    mode = O_RDWR | O_CLOEXEC | O_CREAT;
    g_logI.slogFd = open(g_logI.slogFile, mode, LOG_FILE_O_FLAGS);
    if (g_logI.slogFd < 0) {
        KLOGE("re-open slog file fail errno %d\n", errno);
        return g_logI.slogFd;
    }
    if (fchown(g_logI.slogFd, UID_ROOT, GID_SYSTEM) < 0) {
        KLOGE("chown slog file fail errno %d\n", errno);
    }

    return 0;
}

static int InitLogStruct()
{
    unsigned int size;
    struct stat st;
    int ret, i;

    for (i = 0; i < LOG_PARTITION_SIZE; i++) {
        if (stat(g_logPartition[i], &st)) {
            KLOGE("failed to stat %s\n", g_logPartition[i]);
            continue;
        }
        if (!S_ISDIR(st.st_mode)) {
            KLOGE("%s is not a directory\n", g_logPartition[i]);
            continue;
        }
        size = strlen(g_logPartition[i]) + strlen("f2fs-tools/") + 1;
        g_logI.logDir = malloc(size);
        if (g_logI.logDir == NULL) {
            KLOGE("Log dir name fails to initialize\n");
            return LOG_ERR;
        }
        ret = snprintf_s(g_logI.logDir, size, size - 1, "%s%s", g_logPartition[i], "f2fs-tools/");
        if (ret < 0) {
            KLOGE("Log dir name is oversized\n");
            free(g_logI.logDir);
            g_logI.logDir = NULL;
            continue;
        }
        ret = mkdir(g_logI.logDir, LOG_DIR_MODE);
        if (ret < 0 && errno != EEXIST) {
            KLOGE("mkdir %s fail errno %d\n", g_logI.logDir, errno);
            KLOGE("failed to log into %s ret : %d\n", g_logPartition[i], ret);
            free(g_logI.logDir);
            g_logI.logDir = NULL;
            continue;
        }
        if (chown(g_logI.logDir, UID_ROOT, GID_SYSTEM) < 0) {
            KLOGE("chown log dir fail errno %d\n", errno);
        }
        KLOGI("fsck logging to %s\n", g_logPartition[i]);
        break;
    }
    if (i >= LOG_PARTITION_SIZE) {
        KLOGE("init log fail\n");
        return LOG_ERR;
    }
    return 0;
}

static int InitLogFilenames(int logType, char *name)
{
    unsigned int size;
    int ret = 0;

    g_logI.type = logType;
    size = strlen(g_logI.logDir) + strlen(name) + 1;
    g_logI.slogFile = malloc(size);
    if (g_logI.slogFile == NULL) {
        KLOGE("slogFile name fails to initialize\n");
        return LOG_ERR;
    }
    ret = snprintf_s(g_logI.slogFile, size, size - 1, "%s%s", g_logI.logDir, name);
    if (ret < 0) {
        KLOGE("slogFile name fails to setup, return : %d\n", ret);
        free(g_logI.slogFile);
        g_logI.slogFile = NULL;
        return ret;
    }
    ret = 0;
    size += LOG_SLOG_FILENAME_DIFFERENCE;
    g_logI.slogFileBak = malloc(size);
    if (g_logI.slogFileBak == NULL) {
        KLOGE("slogFileBak name fails to initialize\n");
        free(g_logI.slogFile);
        g_logI.slogFile = NULL;
        return LOG_ERR;
    }
    ret = snprintf_s(g_logI.slogFileBak, size, size - 1, "%s%s%s", g_logI.logDir, name, ".1");
    if (ret < 0) {
        KLOGE("slogFileBak name fails to setup, return : %d\n", ret);
        free(g_logI.slogFile);
        g_logI.slogFile = NULL;
        free(g_logI.slogFileBak);
        g_logI.slogFileBak = NULL;
        return ret;
    }
    return ret;
}

static int InitLogInfo(int logType)
{
    char *name = "";
    int ret = 0;

    /* the file and back file are:
     *   g_logI.logDir/buf
     *   g_logI.logDir/buf".1"
     */
    switch (logType) {
        case LOG_TYP_FSCK:
            name = "fsck.log";
            break;
        case LOG_TYP_DUMP:
            name = "dump.log";
            break;
        case LOG_TYP_DEFRAG:
            name = "defrag.log";
            break;
        case LOG_TYP_RESIZE:
            name = "resize.log";
            break;
        case LOG_TYP_MKFS:
            name = "mkfs.log";
            break;
        default:
            KLOGE("unknown log type %d\n", logType);
            return LOG_ERR;
    }

    ret = InitLogStruct();
    if (ret != 0) {
        return ret;
    }

    ret = InitLogFilenames(logType, name);
    return ret;
}

int SlogInit(int funcType)
{
    int ret;
    printf("======== c.func ======== %d\n", (int)funcType);
    switch (funcType) {
        case FSCK:
            ret = DoSlogInit(LOG_TYP_FSCK);
            printf("======== SlogInit(LOG_TYP_FSCK) ======== %d\n", ret);
            break;
        case DUMP:
            ret = DoSlogInit(LOG_TYP_DUMP);
            printf("======== SlogInit(LOG_TYP_DUMP) ======== %d\n", ret);
            break;
        case DEFRAG:
            ret = DoSlogInit(LOG_TYP_DEFRAG);
            printf("======== SlogInit(LOG_TYP_DEFRAG) ======== %d\n", ret);
            break;
        case RESIZE:
            ret = DoSlogInit(LOG_TYP_RESIZE);
            printf("======== SlogInit(LOG_TYP_RESIZE) ======== %d\n", ret);
            break;
        default:
            ret = -1;
            break;
    }
    return ret;
}

static int DoSlogInit(int logType)
{
    time_t t;
    struct tm *tm;
    char *buf = NULL;
    int mode = O_RDWR | O_CLOEXEC | O_CREAT | O_APPEND;
    int ret = 0;

    if (InitLogInfo(logType) < 0) {
        return LOG_ERR;
    }

    g_logI.slogFd = open(g_logI.slogFile, mode, LOG_FILE_O_FLAGS);
    if (g_logI.slogFd < 0) {
        KLOGE("open slog file [%s] fail errno %d\n, strerror %s", g_logI.slogFile, errno, strerror(errno));
        return g_logI.slogFd;
    }
    if (fchown(g_logI.slogFd, UID_ROOT, GID_SYSTEM) < 0) {
        KLOGE("chown slog file fail errno %d\n", errno);
    }
    g_logI.slogOffset = lseek(g_logI.slogFd, 0, SEEK_END);

    KLOGI("Starting logging to %s, offset %ld\n",
            g_logI.slogFile, g_logI.slogOffset);
    /* get current date
     * the string looks like "=== 2016/03/16 14:01:01 ===", its max
     * length is 32.
     */
    buf = calloc(LOG_SLOG_INIT_MEM_BUF_ELEMENT_NUM, LOG_SLOG_INIT_MEM_BUF_SIZE);
    if (!buf) {
        return LOG_ERR;
    }

    t = time(NULL);
    tm = localtime(&t);
    if (!tm) {
        free(buf);
        return LOG_ERR;
    }
    ret = snprintf_s(buf, LOG_SLOG_INIT_MEM_BUF_SIZE, LOG_SLOG_INIT_MEM_BUF_SIZE - 1,
        "\n=== %d/%02d/%02d %02d:%02d:%02d ===\n",
        LOG_TM_STARTING_YEAR + tm->tm_year, 1 + tm->tm_mon, tm->tm_mday,
        tm->tm_hour, tm->tm_min, tm->tm_sec);
    if (ret < 0) {
        KLOGE("write time stamp string buffer fail errno %d\n", errno);
    }
    ret = write(g_logI.slogFd, buf, strlen(buf));
    if (ret < 0) {
        KLOGE("write time stamp %s fail errno %d\n", buf, errno);
    }
    free(buf);
    return ret;
}

void SlogExit(void)
{
    if (g_logI.slogFd < 0) {
        return;
    }

    if (g_logI.needTruncate && g_logI.slogOffset > 0) {
        if (ftruncate(g_logI.slogFd, g_logI.slogOffset)) {
            KLOGE("ftruncate slog fail\n");
        }
    }

    if (fsync(g_logI.slogFd) < 0) {
        KLOGE("fsync slog fail\n");
    }

    if (g_logI.logDir)
        free(g_logI.logDir);
    if (g_logI.slogFile)
        free(g_logI.slogFile);
    if (g_logI.slogFileBak)
        free(g_logI.slogFileBak);
    if (g_logI.slogFd)
        close(g_logI.slogFd);

    KLOGI("Logging done\n");
}

int SlogInitBuffer(char **buffer, int *bufferSize, const char *fmt, va_list args)
{
    char *buf = NULL;
    int size = LOG_BUF_INITIAL_SIZE;
    int ret = 0;

    buf = malloc(size);
    if (!buf) {
        KLOGE("SlogFixSize fail to malloc buffer, size: %d\n", size);
        return LOG_ERR;
    }

    while (1) {
        ret = memset_s(buf, size, 0, size);
        if (ret != 0) {
            KLOGE("SlogFixSize fail to initialize buffer.\n");
            free(buf);
            return LOG_ERR;
        }

        ret = vsnprintf_s(buf, size, size - 1, fmt, args);
        /* when buf is truncated but return -1 */
        if (ret < 0 && strlen(buf) == size - 1) {
            ret = size - 1;
        }
        if (ret < 0) {
            KLOGE("SlogFixSize fail to format buffer: %d\n", ret);
            free(buf);
            return LOG_ERR;
        }

        if (ret < size - 1 || size + size > LOG_BUF_SIZE_PROTECT_THRESHOLD) {
            /* The buffer size should not exceed the protect threshold.
             * The log output is going to be left as truncated by vsnprintf_s.
             */
            /* we get the whole fmt... */
            size = ret;
            break;
        }

        size += size;
        free(buf);
        buf = malloc(size);
        if (!buf) {
            KLOGE("SlogFixSize fail to malloc buffer, size: %d\n", size);
            return LOG_ERR;
        }
    }

    *bufferSize = size;
    *buffer = buf;
    return LOG_OK;
}

int SlogWrite(const char *fmt, ...)
{
    va_list args;
    char *buf = NULL;
    int size = 0;
    int ret = 0;

    if (g_logI.slogFd < 0) {
        return 0;
    }

    va_start(args, fmt);
    ret = SlogInitBuffer(&buf, &size, fmt, args);
    va_end(args);
    if (ret < 0) {
        KLOGE("SlogInitBuffer fail\n");
        return ret;
    }

    ret = SlogFixSize(size);
    if (ret < 0) {
        KLOGE("SlogFixSize fail\n");
        free(buf);
        return ret;
    }

    ret = write(g_logI.slogFd, buf, size);
    if (ret < 0) {
        KLOGE("write to slog file fail errno %d\n", errno);
    }
    free(buf);
    return ret;
}
