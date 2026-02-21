/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: enhance log records for fsck-tools
 * Create: 2024-3-4
*/
#include "f2fs_dmd_cfg.h"
#include "f2fs_dmd.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "f2fs_log.h"
#include "securec.h"

#ifndef CONF_TARGET_HOST
#define HIEVENT_DRIVER_NODE  "/dev/storage"
#define HM_STORAGE_IOCTL_BASE  'S'
#define HM_STORAGE_CMD_EVENT_REPORT_FSCK 1
#define EVENT_REPORT_FSCK_CMD \
    _IOW(HM_STORAGE_IOCTL_BASE, HM_STORAGE_CMD_EVENT_REPORT_FSCK, struct DmdReport)

enum EVENT_REPORT_FSCK_TYPE {
    EVENT_REPORT_FSCK_TOOLS_ERROR = 1
};

#define FAULT_FUNC_SIZE 20U
#define FAULT_STORE_SIZE 64U /* maximum number of tallied faults */
#define ASSERT_MSG_SIZE 1536U
#define BITS_PER_UINT64 64U
#define LOG2_BITS_PER_UINT64 6U

struct __attribute__((packed)) DmdFault {
    char func[FAULT_FUNC_SIZE]; /* func name buffer size */
    int line;
    unsigned int errnum;
    unsigned int count;
};

#define NR_OVERTIME_THRESHOLD_LEVEL 3U
enum {
    idxSpaceThreshold = 0,
    idxTimeThreshold = 1,
    idxThresholdMax
};
const unsigned long g_overtimeThresholdBySpace[NR_OVERTIME_THRESHOLD_LEVEL][idxThresholdMax] = {
    /* total FS size in MB, Time cost threshold in millisecs */
    {524288uL, 1000uL},
    {1048576uL, 3000uL},
    {2097152uL, 5000uL}
};

struct DmdFault g_dmdErrorStore[FAULT_STORE_SIZE] = {0};
unsigned int g_nrStoredErrors = 0;
struct DmdReport g_dmdReport = {0};
bool g_dmdMarkReport = false;

struct DmdMsg g_reportMsg = {
    .buffer = g_dmdReport.msg,
    .size = FSCK_REPORT_MSG_SIZE,
    .offset = 0
};

char g_dmdAssertMsg[ASSERT_MSG_SIZE] = {0};
struct DmdMsg g_assertMsg = {
    .buffer = g_dmdAssertMsg,
    .size = ASSERT_MSG_SIZE,
    .offset = 0
};

static void DmdSetPropertyBit(unsigned int bit)
{
    g_dmdReport.propBitmap |= bit;
}

static void DmdClearPropertyBit(unsigned int bit)
{
    g_dmdReport.propBitmap &= (~bit);
}

static int DmdMarkReportWrite(void)
{
    if (!g_dmdMarkReport) {
        return DMD_OK;
    }

    int fd = open(HIEVENT_DRIVER_NODE, O_RDWR);
    if (fd < 0) {
        KLOGE("Fail to open hievent node: %d.\n", errno);
        return DMD_ERR;
    }

    if (ioctl(fd, EVENT_REPORT_FSCK_CMD, &g_dmdReport) < 0) {
        KLOGE("Fail to write DmdReportFields with errno: %d.\n", errno);
        close(fd);
        return DMD_ERR;
    }
    close(fd);
    KLOGI("Mark sysfs tools_report success.\n");
    g_dmdMarkReport = false;
    return DMD_OK;
}

static void ReadDeviceState(void)
{
    const char cmdlinePath[] = "/proc/cmdline";
    const char matchStr[] = "ohos.boot.hvb.device_state=locked";
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    FILE *stream = fopen(cmdlinePath, "r");
    if (stream == NULL) {
        return;
    }

    while ((nread = getline(&line, &len, stream)) != -1) {
        if (strstr(line, matchStr) != NULL) {
            DmdSetPropertyBit(FB_LOCKED_FL);
            break;
        }
    }
    if (line) {
        free(line);
    }
    (void)fclose(stream);
}

static void FillReportMsg(void)
{
    unsigned int i = 0;

    while (i < g_nrStoredErrors) {
        PrintToMsg(&g_reportMsg, "[ERR=%d(%s:%d)%u]",
            g_dmdErrorStore[i].errnum, g_dmdErrorStore[i].func,
            g_dmdErrorStore[i].line, g_dmdErrorStore[i].count);
        i++;
    }

    PrintToMsg(&g_reportMsg, "%s", g_assertMsg.buffer);
}

int DmdReport(void)
{
    int ret;

    FillReportMsg();
    ReadDeviceState();
    ret = DmdMarkReportWrite();
    if (ret < 0) {
        KLOGE("Write sysfs tools_report failed\n");
    }

    return ret;
}

static void DmdMarkReport(void)
{
    if (g_dmdMarkReport) {
        return;
    }

    g_dmdMarkReport = true;
}

static int SetErrBitmap(unsigned int errnum)
{
    unsigned int index = errnum >> LOG2_BITS_PER_UINT64;
    uint64_t mask = 1ULL << (errnum & (BITS_PER_UINT64 - 1U));

    if (index >= NR_ERR_BITMAP_UINT64) {
        KLOGE("err=%u larger than bits of errBitmap\n", errnum);
        return DMD_ERR;
    }
    g_dmdReport.errBitmap[index] |= mask;
    return DMD_OK;
}

void DmdInsertError(int type, unsigned int err, const char *func, int line)
{
    unsigned int i;
    int ret = SetErrBitmap(err);
    if (ret < 0) {
        return;
    }

    /* check for similar error occurence */
    for (i = 0; i < g_nrStoredErrors; ++i) {
        if (g_dmdErrorStore[i].errnum == err && g_dmdErrorStore[i].line == line &&
            strncmp(g_dmdErrorStore[i].func, func, FAULT_FUNC_SIZE) == 0) {
                g_dmdErrorStore[i].count++;
                return;
        }
    }

    if (g_nrStoredErrors >= FAULT_STORE_SIZE) {
        KLOGE("DMD more faults than dump storage, DMD MSG: %s:%d %x\n", func, line, err);
        return;
    }

    ret = strncpy_s(g_dmdErrorStore[g_nrStoredErrors].func, FAULT_FUNC_SIZE, func, FAULT_FUNC_SIZE - 1U);
    if (ret) {
        KLOGE("strcpy failed, DMD MSG: %s:%d %x\n", func, line, err);
        return;
    }
    g_dmdErrorStore[g_nrStoredErrors].line = line;
    g_dmdErrorStore[g_nrStoredErrors].errnum = err;
    g_dmdErrorStore[g_nrStoredErrors].count = 1U;
    g_nrStoredErrors++;
    DmdMarkReport();
}

void PrintToMsg(struct DmdMsg *dmdMsg, const char *fmt, ...)
{
    va_list argList;
    va_start(argList, fmt);

    if (dmdMsg->offset < dmdMsg->size - 1U) {
        int ret = vsnprintf_s(dmdMsg->buffer + dmdMsg->offset,
                                    dmdMsg->size - dmdMsg->offset,
                                    dmdMsg->size - dmdMsg->offset - 1U,
                                    fmt, argList);
        if (ret > 0) {
            dmdMsg->offset += (unsigned int)ret;
        } else {
            /* Ignore errors, print to buffer as much as buffer size */
            dmdMsg->offset = strlen(dmdMsg->buffer);
        }
    }

    va_end(argList);
    (void)argList;
}

void DmdCheckCostTime(const char *func, int line)
{
    unsigned int iLevel;
    unsigned long totalSize = g_dmdReport.usedSpace + g_dmdReport.freeSpace;

    for (iLevel = 0; iLevel < NR_OVERTIME_THRESHOLD_LEVEL; iLevel++) {
        if (totalSize <= g_overtimeThresholdBySpace[iLevel][idxSpaceThreshold]) {
            if (g_dmdReport.costTime > g_overtimeThresholdBySpace[iLevel][idxTimeThreshold]) {
                DmdInsertError(LOG_TYP_FSCK, PR_FSCK_TIME_OVERCOST, func, line);
            }
            break;
        }
    }

    DmdMarkReport(); /* Currently we report everytime for statistics */
}

#else

int DmdReport(void)
{
    return DMD_OK;
}

#endif
