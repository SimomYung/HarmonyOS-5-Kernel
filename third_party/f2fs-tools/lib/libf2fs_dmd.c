/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: enhance log records for fsck-tools
 * Create: 2024-3-4
*/
#include "f2fs_dmd_cfg.h"

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "f2fs_log.h"
#include "securec.h"

#ifndef CONF_TARGET_HOST
#define FSCK_REPORT_MSG_SIZE   256
struct dmd_report_msg {
    int type;
    char report_buf[FSCK_REPORT_MSG_SIZE];
};

#define HIEVENT_DRIVER_NODE  "/dev/storage"
#define HM_STORAGE_IOCTL_BASE  'S'
#define HM_STORAGE_CMD_EVENT_REPORT_FSCK 1
#define EVENT_REPORT_FSCK_CMD \
	_IOW(HM_STORAGE_IOCTL_BASE, HM_STORAGE_CMD_EVENT_REPORT_FSCK, sizeof(struct dmd_report_msg))

enum EVENT_REPORT_FSCK_TYPE {
	EVENT_REPORT_FSCK_TOOLS_ERROR = 1
};

/*
* The buffer size grows with a growth factor of 1.5.
* With a factor which is LESS THAN TWO can make it possiable to reuse allocated memory before.
*/

char *g_dmdInsertBuf = NULL;
int g_dmdWriteSize = 0;
int g_dmdBufSize = 0;
bool g_dmdMarkReport = false;
void DmdMarkReport(void);

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

    struct dmd_report_msg report_msg = {0};
    report_msg.type = EVENT_REPORT_FSCK_TOOLS_ERROR;
    (void)strncpy_s(report_msg.report_buf, FSCK_REPORT_MSG_SIZE, g_dmdInsertBuf,
        (g_dmdWriteSize >= FSCK_REPORT_MSG_SIZE) ? (FSCK_REPORT_MSG_SIZE - 1) : g_dmdWriteSize);
    report_msg.report_buf[FSCK_REPORT_MSG_SIZE - 1] = '\0';
    if (ioctl(fd, EVENT_REPORT_FSCK_CMD, &report_msg) < 0) {
        close(fd);
        KLOGE("Fail to write sysfs tools_report with errno: %d.\n", errno);
        return DMD_ERR;
    }
    close(fd);
    KLOGI("Mark sysfs tools_report success.\n");
    g_dmdMarkReport = false;
    return DMD_OK;
}

int DmdReport()
{
    int ret = DMD_OK;

    ret = DmdMarkReportWrite();
    if (ret < 0) {
        KLOGE("Write sysfs tools_report failed\n");
    }

    return ret;
}

void DmdInsertError(int type, int err, const char *func, int line)
{
    if (g_dmdWriteSize >= FSCK_REPORT_MSG_SIZE - 1) {
        KLOGE("DMD buffer is over, DMD MSG: %s:%d %x;\n", func, line, err);
        return;
    }

    int dmdInsertSize = 0;
    char *bufPtr;
    int remainLen = 0;

    if (type < LOG_TYP_FSCK || type >= LOG_TYP_MAX) {
        KLOGE("Unknown log type %d\n", type);
        return;
    }

    if (g_dmdInsertBuf == NULL) {
        g_dmdInsertBuf = malloc(FSCK_REPORT_MSG_SIZE);
        if (!g_dmdInsertBuf) {
            KLOGE("DMD buffer fail to allocated. DMD MSG: %s:%d %x;\n", func, line, err);
            return;
        }
        g_dmdBufSize = FSCK_REPORT_MSG_SIZE;
        g_dmdWriteSize = 0;
    }

    remainLen = g_dmdBufSize - g_dmdWriteSize;
    bufPtr = g_dmdInsertBuf + g_dmdWriteSize;
    dmdInsertSize = snprintf_s(bufPtr, remainLen, remainLen - 1, "%s:%d %x;", func, line, err);
    if (dmdInsertSize == -1) {
        g_dmdWriteSize = FSCK_REPORT_MSG_SIZE - 1;
    } else {
        g_dmdWriteSize += dmdInsertSize;
    }
    DmdMarkReport();
}

void DmdMarkReport()
{
    if (g_dmdMarkReport) {
        return;
    }

    g_dmdMarkReport = true;
}

#else

int DmdReport()
{
    return DMD_OK;
}

void DmdInsertError(int UNUSED(type), int UNUSED(err), const char *UNUSED(func), int UNUSED(line))
{
}

int DmdMarkReport()
{
	return DMD_OK;
}

#endif
