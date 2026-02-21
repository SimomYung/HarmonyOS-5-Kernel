/**
 * @file mem_hm_leakcheck.c
 * @copyright Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * @brief mem_hm_leakcheck 内存健康管理的内存泄露检测功能模块源文件。
 * @details
 * @author anonymous
 * @date 2021-10-12
 * @version v1.0.0
 * *******************************************************************************************
 * @par 修改日志：
 * <table>
 * <tr><th>Date        <th>Version  <th>Author    <th>Description
 * <tr><td>2021-10-12  <td>1.0.0    <td>anonymous <td>创建初始版本
 * </table>
 *
 * *******************************************************************************************
 */
#ifdef CONFIG_DEBUG_BUILD
#include <execinfo.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include "allocator.h"
#include "mem_hm_inner.h"
#include "v_legacyavl.h"
#include "mem_hm_log_inner.h"

#ifdef _MUSL_SOURCE
#include "libc.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

#define MEM_HM_PROC_STAT_KEY_WORD 4         /* 需要跳过stat文件key字节的长度 */

#define MEM_HM_BYTE_ALIGN 4                 /* 扫描时按照4字节对齐进行内存内容的读取 */

#define MEM_HM_RESOURCEBUF_LENGTH 1024      /* 资源池buffer的长度，用于读取文件内容 */

#define MEM_HM_TMPBUF_LENGTH 256            /* 临时资源池buffer的长度，用于承载文件路径内容 */

/* 底层libc库是glibc的情况下，需要定义PAGE_SIZE宏 */
#ifndef _MUSL_SOURCE
#define PAGE_SIZE (getpagesize())
#endif

/* 内存扫描区域链表节点结构体 */
struct TagMemHMScanArea {
    bool isSysStack;                /* 是否是系统栈内存区域 */
    uint8_t reserved[7];            /* 保留字段，用于对齐 */
    uintptr_t beginAddr;            /* 扫描区域段的起始地址 */
    uintptr_t endAddr;              /* 扫描区域段的终止地址 */
    uintptr_t tmpAddr;              /* 扫描区域段的临时地址，用于缩小栈地址区域范围逻辑中 */
    struct TagMemHMScanArea *next;  /* 指向下一个扫描区域段指针 */
};

typedef struct TagMemHMScanArea MemHMScanArea;

/* maps信息中每一行的数据承载结构体 */
typedef struct {
    uintptr_t beginAddr;            /* 该段的起始地址 */
    uintptr_t endAddr;              /* 该段的终止地址 */
    uintptr_t excurs;
    uint32_t mainEquip;             /* 主设备号 */
    uint32_t childEquip;            /* 次设备号 */
    uint32_t node;                  /* 当前可执行文件的文件索引节点号 */
    char readAble;                  /* 是否有可读权限 */
    char writeAble;                 /* 是否有可写权限 */
    char excuteAble;                /* 是否有可执行权限 */
    char private;                   /* 是否私有 */
    char *notes;                    /* 该maps行的内容 */
} MemHMMapsInfo;

/* 内存泄露AVL树遍历钩子函数结构体 */
typedef int32_t (*MemHMLeakTreeWalkFunc)(AVL_NODE_S *node, void *para);

/* 内存泄露DFX功能检测状态枚举 */
typedef enum {
    MEM_LEAK_STATE_UNSTART = 0,     /* 内存泄露检测未开始 */
    MEM_LEAK_STATE_DOING = 1,       /* 内存泄露检测进行中 */
    MEM_LEAK_STATE_FINISH = 2,      /* 内存泄露检测已完成 */
    MEM_LEAK_STATE_BUTT
} MemLeakState;

#ifdef OS_HONGMENG
#ifdef __clang__
#else
extern char *__data_start;    /* begin of data segment */
#endif
#else
extern char *data_start;      /* begin of data segment */
#endif

extern char *_end;            /* end of bss segment */

/* 内存泄露检测后泄露的内存AVL树根节点 */
MEM_HM_STATIC AVL_NODE_S *g_memLeakTreeHead = NULL;

/* 内存泄露检测状态标志 */
MEM_HM_STATIC int32_t g_memLeakState = MEM_LEAK_STATE_UNSTART;

/* 内存泄露检测后泄露的内存块数量 */
MEM_HM_STATIC int32_t g_memLeakCount = 0;

/* 内存泄露检测后泄露的内存块总大小 */
MEM_HM_STATIC size_t g_memLeakSize = 0;

MEM_HM_STATIC int32_t MemHMLeakScanResourceGet(char **buffer, char **notes, char **tmpBuffer)
{
    char *totalBuffer;

    totalBuffer = (char *)ALLOCATOR_ALLOC_INNER(3 * MEM_HM_RESOURCEBUF_LENGTH); /* 3代表数量，3个buffer资源池 */
    if (totalBuffer == NULL) {
        return MEM_HM_ERRNO_NOMEM;
    }

    (void)memset(totalBuffer, 0, 3 * MEM_HM_RESOURCEBUF_LENGTH); /* 3代表数量 */
    *buffer = totalBuffer;
    *notes = (char *)((uintptr_t)totalBuffer + MEM_HM_RESOURCEBUF_LENGTH);
    *tmpBuffer = (char *)((uintptr_t)totalBuffer + 2 * MEM_HM_RESOURCEBUF_LENGTH); /* 2指跳过两个length到tmpBuffer地址 */

    return MEM_HM_OK;
}

MEM_HM_STATIC int32_t MemHMCurStkAddrGetFromStkBuf(char *buffer, uint32_t bufferLen, uint64_t *curStackAddr)
{
    int32_t ret;
    char *statBuffer;
    uint64_t stackAddr;

    /* 找到")"的位置 */
    statBuffer = strrchr(buffer, ')');
    if (statBuffer == NULL) {
        return MEM_HM_ERRNO_INVAL;
    }

    statBuffer += MEM_HM_PROC_STAT_KEY_WORD;
    if (statBuffer > (buffer + bufferLen)) {
        return MEM_HM_ERRNO_INVAL;
    }

#if defined(__WORDSIZE) && __WORDSIZE != 64
    ret = sscanf(statBuffer, "%*llu %*llu %*llu %*llu %*llu %*llu %*llu %*llu %*llu %*llu %*llu %*llu %*llu "
                     "%*llu %*llu %*llu %*llu %*llu %*llu %*llu %*llu %*llu %*llu %*llu %*llu %llu", &stackAddr);
#else
    ret = sscanf(statBuffer, "%*lu %*lu %*lu %*lu %*lu %*lu %*lu %*lu %*lu %*lu %*lu %*lu %*lu "
                     "%*lu %*lu %*lu %*lu %*lu %*lu %*lu %*lu %*lu %*lu %*lu %*lu %lu", &stackAddr);
#endif
    if (ret != 1) {
        return MEM_HM_ERRNO_INVAL;
    }

    *curStackAddr = stackAddr;
    return MEM_HM_OK;
}

MEM_HM_STATIC int32_t MemHMCurStkAddrGet(uint32_t pid, uintptr_t *curStkAddr)
{
    FILE *fp;
    int32_t ret;
    char *tmpBuffer;
    uint64_t curStackAddr;
    char buffer[MEM_HM_TMPBUF_LENGTH] = {0};

    (void)snprintf(buffer, MEM_HM_TMPBUF_LENGTH, "/proc/%u/stat", pid);
    fp = fopen(buffer, "r");
    if (fp == NULL) {
        return MEM_HM_ERRNO_INVAL;
    }

    tmpBuffer = (char *)ALLOCATOR_ALLOC_INNER(MEM_HM_RESOURCEBUF_LENGTH);
    if (tmpBuffer == NULL) {
        (void)fclose(fp);
        return MEM_HM_ERRNO_NOMEM;
    }

    (void)memset(tmpBuffer, 0, MEM_HM_RESOURCEBUF_LENGTH);
    rewind(fp);

    if (fgets(tmpBuffer, MEM_HM_RESOURCEBUF_LENGTH, fp) == NULL) {
        (void)fclose(fp);
        (void)ALLOCATOR_FREE_INNER(tmpBuffer);
        return MEM_HM_ERRNO_INVAL;
    }

    (void)fclose(fp);
    fp = NULL;

    ret = MemHMCurStkAddrGetFromStkBuf(tmpBuffer, MEM_HM_RESOURCEBUF_LENGTH, &curStackAddr);
    (void)ALLOCATOR_FREE_INNER(tmpBuffer);
    if (ret != MEM_HM_OK) {
        return ret;
    }

    (*curStkAddr) = (uintptr_t)curStackAddr;
    return MEM_HM_OK;
}

MEM_HM_STATIC bool MemHMLeakScanIsDllSeg(const MemHMMapsInfo *mapsInfo, uintptr_t endAddr)
{
    bool isDllSegmentEx = false;
    static bool isDllSegment = false;
#ifdef OS_HONGMENG
    static bool isPriorRPSOSegment = false;
#endif

    /* 鸿蒙系统maps信息差异化处理，加OS_HONGMENG宏隔离 */
#ifdef OS_HONGMENG
    if ((isDllSegment) && (strcmp(mapsInfo->notes, "anonymous") == 0) && (mapsInfo->beginAddr == endAddr)) {
        isDllSegmentEx = true;
    }
#else
    if ((isDllSegment) && (strlen(mapsInfo->notes) == 0) && (mapsInfo->beginAddr == endAddr)) {
        isDllSegmentEx = true;
    }
#endif

    isDllSegment = false;
    if ((mapsInfo->writeAble == 'w') && (mapsInfo->excuteAble == '-') && (strlen(mapsInfo->notes) != 0)) {
        if (strstr(mapsInfo->notes, ".so") != NULL) {
            isDllSegment = true;
        }
#ifdef OS_HONGMENG
        if (strcmp(mapsInfo->notes, "file") == 0) {
            isDllSegment = true;
        }

        if ((isPriorRPSOSegment) && (strcmp(mapsInfo->notes, "anonymous") == 0)) {
            isDllSegment = true;
        }
#endif
    }

#ifdef OS_HONGMENG
    if ((mapsInfo->readAble == 'r') && (mapsInfo->writeAble == '-') &&
        (mapsInfo->excuteAble == '-') && (strstr(mapsInfo->notes, ".so") != NULL)) {
        isPriorRPSOSegment = true;
    } else {
        isPriorRPSOSegment = false;
    }
#endif

    return ((isDllSegment) || (isDllSegmentEx));
}

MEM_HM_STATIC int32_t MemHMLeakScanDllBeginAddrSet(const MemHMMapsInfo *mapsInfo, bool isDllSeg, bool isStkSeg,
    MemHMScanArea **area)
{
    MemHMScanArea *newArea;

    if (isDllSeg) {
        newArea = (MemHMScanArea *)ALLOCATOR_ALLOC_INNER(sizeof(MemHMScanArea));
        if (newArea == NULL) {
            return MEM_HM_ERRNO_NOMEM;
        }
        memset(newArea, 0, sizeof(MemHMScanArea));
        newArea->beginAddr = mapsInfo->beginAddr;
        *area = newArea;
    } else if (isStkSeg) {
        newArea = (MemHMScanArea *)ALLOCATOR_ALLOC_INNER(sizeof(MemHMScanArea));
        if (newArea == NULL) {
            return MEM_HM_ERRNO_NOMEM;
        }
        memset(newArea, 0, sizeof(MemHMScanArea));
        newArea->beginAddr = mapsInfo->beginAddr; /* 该起始地址后面在task里面理应再被赋值，以缩小范围，如果sp获取失败则不变 */
        *area = newArea;
    } else {
        return MEM_HM_ERRNO_INVAL;
    }

    return MEM_HM_OK;
}

MEM_HM_STATIC int32_t MemHMLeakScanProcMapsGet(char *buffer, char *notes, MemHMScanArea *area, FILE *mapFile,
    uintptr_t curStkAddr)
{
    int32_t ret;
    bool isBssSeg = false;
    bool isDllSeg = false;
    bool isStkSeg = false;
    MemHMMapsInfo mapsInfo;
    uintptr_t stackEnd = 0;
    bool maybeStack = false;   /* false 意味着这个段是bss段，true 意味着是栈段 */
    MemHMScanArea *areaLink = area;

    rewind(mapFile);
    for (;;) {
        (void)memset(buffer, 0, MEM_HM_RESOURCEBUF_LENGTH);
        /* read file failure */
        if (!fgets(buffer, MEM_HM_RESOURCEBUF_LENGTH, mapFile)) {
            /* the end of file */
            if (feof(mapFile) != 0) {
                break;
            } else {
                return MEM_HM_ERRNO_INVAL;
            }
        }

        (void)memset(notes, 0, MEM_HM_RESOURCEBUF_LENGTH);
        mapsInfo.notes = notes;
#if defined(__WORDSIZE) && (__WORDSIZE != 64)
        ret = sscanf(buffer, "%x-%x %c%c%c%c %x %x:%x %d %1023s", &mapsInfo.beginAddr, &mapsInfo.endAddr,
            &mapsInfo.readAble, &mapsInfo.writeAble, &mapsInfo.excuteAble, &mapsInfo.private,
            &mapsInfo.excurs, &mapsInfo.mainEquip, &mapsInfo.childEquip, &mapsInfo.node, mapsInfo.notes);
#else
        ret = sscanf(buffer, "%lx-%lx %c%c%c%c %lx %x:%x %d %1023s", &mapsInfo.beginAddr, &mapsInfo.endAddr,
            &mapsInfo.readAble, &mapsInfo.writeAble, &mapsInfo.excuteAble, &mapsInfo.private,
            &mapsInfo.excurs, &mapsInfo.mainEquip, &mapsInfo.childEquip, &mapsInfo.node, mapsInfo.notes);
#endif
        if ((ret != 11) && (ret != 10)) { /* 11和10表示sscanf_s要读取字段个数 */
            return MEM_HM_ERRNO_INVAL;
        }

        /* maps规则认为无可读、可写、可执行权限且长度为4K的段可能是bss段 */
        isBssSeg = ((mapsInfo.readAble == '-') && (mapsInfo.writeAble == '-') && (mapsInfo.excuteAble == '-') &&
            (mapsInfo.endAddr - mapsInfo.beginAddr == 4096)); /* 4096: 4K */
        if (isBssSeg) {
            maybeStack = true;
            stackEnd = mapsInfo.endAddr;
            continue;
        }

        /* 获取动态库数据段地址范围 */
        isDllSeg = MemHMLeakScanIsDllSeg(&mapsInfo, area->endAddr);
        isStkSeg = ((mapsInfo.readAble == 'r') && (mapsInfo.writeAble == 'w') && (mapsInfo.node == 0) &&
            ((strcmp("[stack]", mapsInfo.notes) == 0) || ((curStkAddr >= mapsInfo.beginAddr) &&
            (curStkAddr < mapsInfo.endAddr)) || ((maybeStack) && (mapsInfo.beginAddr == stackEnd))));
        maybeStack = false;
        stackEnd = 0;
        ret = MemHMLeakScanDllBeginAddrSet(&mapsInfo, isDllSeg, isStkSeg, &area);
        if (ret == MEM_HM_ERRNO_NOMEM) {
            return ret;
        } else if (ret == MEM_HM_ERRNO_INVAL) {
            continue;
        }

        if ((strcmp("[stack]", mapsInfo.notes) == 0) || ((curStkAddr >= mapsInfo.beginAddr) &&
            (curStkAddr < mapsInfo.endAddr))) {
            area->isSysStack = true;
            area->beginAddr = mapsInfo.beginAddr;
        } else {
            area->isSysStack = false;
        }

        area->tmpAddr = mapsInfo.beginAddr;
        area->endAddr = mapsInfo.endAddr;
        area->next = NULL;
        /* stack segment , add to the link */
        areaLink->next = area;
        areaLink = areaLink->next;
    }

    return MEM_HM_OK;
}

MEM_HM_STATIC int32_t MemHMLeakScanProcMapsAnalyze(char *buffer, char *notes, MemHMScanArea *area)
{
    int32_t ret;
    uint32_t pid;
    FILE *mapFile;
    uintptr_t curStkAddr;

    pid = (uint32_t)getpid();
    /* 获取系统栈指针，这里是通过stat文件获取的，很可能获取成0，获取不出来，有误报 */
    ret = MemHMCurStkAddrGet(pid, &curStkAddr);
    if (ret != MEM_HM_OK) {
        return ret;
    }

    (void)snprintf(buffer, MEM_HM_RESOURCEBUF_LENGTH, "/proc/%u/maps", pid);
    mapFile = fopen(buffer, "r");
    if (mapFile == NULL) {
        return MEM_HM_ERRNO_INVAL;
    }

    ret = MemHMLeakScanProcMapsGet(buffer, notes, area, mapFile, curStkAddr);
    (void)fclose(mapFile);

    return ret;
}

MEM_HM_STATIC int32_t MemHMTaskStatInfoGet(char *statBuffer, uint64_t *tmp25, uint64_t *tmp26)
{
    int32_t ret;
    uint64_t tmp[32]; /* 32 是指 /proc/%u/task/%s/stat 文件解析后的元素最长都不会超过32，因此用32 */

    /* 第26个成员是线程的esp值 */
#if defined(__WORDSIZE) && (__WORDSIZE != 64)
    ret = sscanf(statBuffer, "%llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu "
        /* 2、3、4数组下标 */
        "%llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu", &tmp[1], &tmp[2], &tmp[3], &tmp[4],
        /* 5、6、7、8、9、10、11、12、13数组下标 */
        &tmp[5], &tmp[6], &tmp[7], &tmp[8], &tmp[9], &tmp[10], &tmp[11], &tmp[12], &tmp[13],
        /* 14、15、16、17、18、19、20、21数组下标 */
        &tmp[14], &tmp[15], &tmp[16], &tmp[17], &tmp[18], &tmp[19], &tmp[20], &tmp[21],
        /* 22、23、24、25、26数组下标 */
        &tmp[22], &tmp[23], &tmp[24], &tmp[25], &tmp[26]);
#else
    ret = sscanf(statBuffer, "%lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu "
        /* 2、3、4、5、6数组下标 */
        "%lu %lu %lu %lu %lu %lu %lu", &tmp[1], &tmp[2], &tmp[3], &tmp[4], &tmp[5], &tmp[6],
        /* 7、8、9、10、11、12、13、14、15数组下标 */
        &tmp[7], &tmp[8], &tmp[9], &tmp[10], &tmp[11], &tmp[12], &tmp[13], &tmp[14], &tmp[15],
        /* 16、17、18、19、20、21、22、23数组下标 */
        &tmp[16], &tmp[17], &tmp[18], &tmp[19], &tmp[20], &tmp[21], &tmp[22], &tmp[23],
        &tmp[24], &tmp[25], &tmp[26]); /* 24、25、26数组下标 */
#endif
    if (ret < 26) { /* 26 是指26个数组元素，如果sscanf_s的返回值小于26，说明该函数执行异常 */
        return MEM_HM_ERRNO_INVAL;
    }

    *tmp25 = tmp[25];   /* 25:该任务在虚拟地址空间的栈的结束地址 */
    *tmp26 = tmp[26];   /* 26:记录esp的当前值 */

    return MEM_HM_OK;
}

MEM_HM_STATIC void MemHMScanAreaUpdate(const MemHMScanArea **areaLinkHead, uint64_t tmp25, uint64_t tmp26,
    uintptr_t threadId)
{
    MemHMScanArea *linkArea;

    /* 第一项不是堆栈段，所以从第二项开始 */
    linkArea = (*areaLinkHead)->next;
    while (linkArea != NULL) {
        /* find the least stack_base in which are bigger than   */
        if ((linkArea->endAddr > (uintptr_t)tmp26) && (linkArea->tmpAddr < (uintptr_t)tmp26)) {
            if (!linkArea->isSysStack) {
                if (threadId == (uint32_t)-1) {
                    break;
                }

                linkArea->beginAddr = (uintptr_t)tmp26; /* 26:记录esp的当前值 */
                if (threadId < linkArea->endAddr) {
                    linkArea->endAddr = threadId;
                }

                break;
            }

            linkArea->beginAddr = (uintptr_t)tmp26; /* 26:记录esp的当前值 */
            linkArea->endAddr = (uintptr_t)tmp25; /* 25:该任务在虚拟地址空间的栈的结束地址 */
            break;
        }

        linkArea = linkArea->next;
    }
}

MEM_HM_STATIC int32_t MemHMLeakScanProcTaskAnalyze(char *buffer, char *tmpBuffer, const MemHMScanArea **areaLinkHead)
{
    FILE *fp;
    int32_t ret;
    uint32_t pid;
    char *statBuffer;
    uint32_t threadNum;
    uintptr_t threadId;
    uint64_t tmp25, tmp26;
    struct dirent **namelist = NULL;

    pid = (uint32_t)getpid();
    /* 获取每个线程的stat文件 */
    (void)snprintf(buffer, MEM_HM_RESOURCEBUF_LENGTH, "/proc/%u/task", pid);
    threadNum = (uint32_t)scandir(buffer, &namelist, 0, 0);
    if ((threadNum == 0) || (threadNum == (uint32_t)(-1))) {
        return MEM_HM_ERRNO_INVAL;
    }

    while ((threadNum--) != 0) {
        /* 当前目录和父目录不可用 */
        if ((strcmp(namelist[threadNum]->d_name, ".") != 0) && (strcmp(namelist[threadNum]->d_name, "..") != 0)) {
            ret = snprintf(buffer, MEM_HM_RESOURCEBUF_LENGTH, "/proc/%u/task/%s/stat",
                pid, namelist[threadNum]->d_name);
            fp = fopen(buffer, "r");
            if ((ret < 0) || (fp == NULL)) {
                /* 线程可能并发退出了，此处直接continue，若如此，此处可能有误报 */
                continue;
            }

            rewind(fp);
            (void)memset((void *)tmpBuffer, 0, MEM_HM_RESOURCEBUF_LENGTH);
            if (fgets(tmpBuffer, MEM_HM_RESOURCEBUF_LENGTH, fp) == NULL) {
                (void)fclose(fp);
                /* 线程可能并发退出了，此处直接continue，若如此，此处可能有误报 */
                continue;
            }

            (void)fclose(fp);
            fp = NULL;
            statBuffer = strrchr(tmpBuffer, ')');
            if (statBuffer == NULL) {
                return MEM_HM_ERRNO_INVAL;
            }

            statBuffer += MEM_HM_PROC_STAT_KEY_WORD;
            ret = MemHMTaskStatInfoGet(statBuffer, &tmp25, &tmp26);
            if (ret != MEM_HM_OK) {
                return ret;
            }

            threadId = (uintptr_t)atoi(namelist[threadNum]->d_name);
            MemHMScanAreaUpdate(areaLinkHead, tmp25, tmp26, threadId);
        }

        free(namelist[threadNum]);
        namelist[threadNum] = NULL;
    }

    free(namelist);
    namelist = NULL;
    return MEM_HM_OK;
}

MEM_HM_STATIC void MemHMLeakScanResourceFree(const char *buffer, const char *notes, const char *tmpBuffer)
{
    (void)notes;
    (void)tmpBuffer;
    ALLOCATOR_FREE_INNER((void *)buffer);
}

MEM_HM_STATIC void MemHMScanAreaLinkDestory(MemHMScanArea *areaLinkHead)
{
    MemHMScanArea *areaNode = NULL;
    MemHMScanArea *areaLinkTmp = areaLinkHead;

    while (areaLinkTmp != NULL) {
        areaNode = areaLinkTmp;
        areaLinkTmp = areaLinkTmp->next;
        ALLOCATOR_FREE_INNER(areaNode);
    }
}

MEM_HM_STATIC int32_t MemHMScanAreaLinkGet(MemHMScanArea **areaLinkHead)
{
    int32_t ret;
    char *notes;
    char *buffer;
    char *tmpBuffer;
    MemHMScanArea *area;

    /* 扫描过程中内存资源准备 */
    ret = MemHMLeakScanResourceGet(&buffer, &notes, &tmpBuffer);
    if (ret != MEM_HM_OK) {
        return ret;
    }

    /* 先准备数据段地址范围 */
    area = (MemHMScanArea *)ALLOCATOR_ALLOC_INNER(sizeof(MemHMScanArea));
    if (area == NULL) {
        MemHMLeakScanResourceFree(buffer, notes, tmpBuffer);
        return MEM_HM_ERRNO_NOMEM;
    }
#ifdef OS_HONGMENG
#ifdef __clang__
    area->beginAddr = (uintptr_t)NULL;
#else
    area->beginAddr = (uintptr_t)&__data_start;
#endif
#else
    area->beginAddr = (uintptr_t)&data_start;
#endif
    area->endAddr = (uintptr_t)&_end;
    area->tmpAddr = (uintptr_t)NULL;
    area->isSysStack = false;
    area->next = NULL;
    *areaLinkHead = area;

    ret = MemHMLeakScanProcMapsAnalyze(buffer, notes, area);
    if (ret != MEM_HM_OK) {
        MemHMScanAreaLinkDestory(*areaLinkHead);
        MemHMLeakScanResourceFree(buffer, notes, tmpBuffer);
        return ret;
    }

    ret = MemHMLeakScanProcTaskAnalyze(buffer, tmpBuffer, (const MemHMScanArea **)areaLinkHead);
    if (ret != MEM_HM_OK) {
        MemHMScanAreaLinkDestory(*areaLinkHead);
    }
    MemHMLeakScanResourceFree(buffer, notes, tmpBuffer);

    return ret;
}

MEM_HM_STATIC int32_t MemHMMemTreeBuild(void *addr, size_t size, void *node)
{
    void *userAddr;
    AVL_NODE_S *tmpNode;
    AVL_NODE_S *head = *(AVL_NODE_S **)node;
    (void)size;

    if (*(uint32_t *)addr == ALLOCATOR_DFX_HEAD_MAGIC) {
        /* 获取真实的用户获取到的地址 */
        userAddr = (void *)((uintptr_t)addr + ALLOCATOR_DFX_HEAD_SIZE);
    } else if (((*(uint32_t *)addr) & ALLOCATOR_DFX_CHECK_MASK) == ALLOCATOR_DFX_HEAD_OFFSET_MAGIC) {
        /* memalign、valloc场景，获取真实的用户获取到的地址 */
        userAddr = (void *)((uintptr_t)addr + (((*(uint32_t *)addr) & ~ALLOCATOR_DFX_CHECK_MASK) >> SHIFT_BIT));
    } else {
        /* 此块内存发生了破坏，此处采用跳过策略，继续扫描其他内存块 */
        return MEM_HM_OK;
    }

    tmpNode = ALLOCATOR_ALLOC_INNER(sizeof(AVL_NODE_S));
    if (tmpNode == NULL) {
        return MEM_HM_ERR;
    }

    memset(tmpNode, 0, sizeof(AVL_NODE_S));
    tmpNode->ulHandle = (uintptr_t)userAddr;
    userAddr = NULL;
    head = VOS_AVLAdd(head, tmpNode, NULL);
    *(AVL_NODE_S **)node = head;

    return MEM_HM_OK;
}

MEM_HM_STATIC int32_t MemHMScanMemTreeGet(AVL_NODE_S **memTreeHead)
{
    int32_t ret;
    AVL_NODE_S *head = NULL;

    ret = ALLOCATOR_ALL_MEM_TRAVERSE((void *)&head, MemHMMemTreeBuild, false);
    if (ret != MEM_HM_OK) {
        return MEM_HM_ERR;
    }

    *memTreeHead = head;
    return MEM_HM_OK;
}

MEM_HM_STATIC size_t MemHMUserSizeGet(void *userAddr)
{
    size_t *sizeAddr;
    uint32_t *offsetFlagAddr;

    offsetFlagAddr = (uint32_t *)((uintptr_t)(userAddr) - sizeof(uint32_t));
    if ((*offsetFlagAddr & ALLOCATOR_DFX_CHECK_MASK) == ALLOCATOR_DFX_OFFSET_MAGIC) {
        sizeAddr = (size_t *)((uintptr_t)(userAddr) - (((*offsetFlagAddr) & ~ALLOCATOR_DFX_CHECK_MASK) >> SHIFT_BIT) +
            sizeof(uint32_t));
    } else if ((*offsetFlagAddr & ALLOCATOR_DFX_CHECK_MASK) == ALLOCATOR_DFX_STACK_END_MAGIC) {
        sizeAddr = (size_t *)((uintptr_t)(userAddr) - ALLOCATOR_DFX_HEAD_SIZE + sizeof(uint32_t));
    } else {
        /* 中魔术字被破坏，返回0，表示无效用户内存长度 */
        return 0;
    }

    return *sizeAddr;
}

MEM_HM_STATIC void *MemHMCallStackAddrGet(void *userAddr)
{
    void *stackAddr = NULL;
    uint32_t *offsetFlagAddr;

    offsetFlagAddr = (uint32_t *)((uintptr_t)(userAddr) - sizeof(uint32_t));
    if ((*offsetFlagAddr & ALLOCATOR_DFX_CHECK_MASK) == ALLOCATOR_DFX_OFFSET_MAGIC) {
        stackAddr = (void *)((uintptr_t)(userAddr) - (((*offsetFlagAddr) & ~ALLOCATOR_DFX_CHECK_MASK) >> SHIFT_BIT) +
            sizeof(uint32_t) + sizeof(size_t));
    } else if ((*offsetFlagAddr & ALLOCATOR_DFX_CHECK_MASK) == ALLOCATOR_DFX_STACK_END_MAGIC) {
        stackAddr = (void *)((uintptr_t)(userAddr) - sizeof(uint32_t) - STACK_LEVEL * sizeof(void *));
    } else {
        /* 中魔术字被破坏，返回NULL，表示无效调用栈地址 */
    }

    return stackAddr;
}

MEM_HM_STATIC int32_t MemHMScanLoop(uintptr_t startAddr, uintptr_t endAddr, AVL_NODE_S **memTreeHead,
                                    MemHMScanArea **areaEnd, MemHMScanArea **nextArea)
{
    size_t tmpSize;
    AVL_NODE_S tmpNode;
    AVL_NODE_S *findNode;
    MemHMScanArea *tmpAreaEnd;
    uintptr_t loop = startAddr;

    /* 4字节的内容可能是指向mem的地址 */
    for (; loop < endAddr; loop += MEM_HM_BYTE_ALIGN) {
        /* 无可达节点 */
        if (*memTreeHead == NULL) {
            break;
        }

        tmpNode.ulHandle = *((uintptr_t *)loop);
        /* 查找一个可到达的节点 */
        findNode = VOS_AVLFind((AVL_NODE_S *)(*memTreeHead), &tmpNode, NULL);
        if (findNode == NULL) {
            continue;
        }

        /* 获取一下对应mem的size */
        tmpSize = MemHMUserSizeGet((void *)(findNode->ulHandle));
        if (tmpSize != 0) {
            /* malloc用于保存新的可访问节点并添加到扫描链表 */
            tmpAreaEnd = *areaEnd;
            tmpAreaEnd->next = (MemHMScanArea *)ALLOCATOR_ALLOC_INNER(sizeof(MemHMScanArea));
            if (tmpAreaEnd->next == NULL) {
                return MEM_HM_ERRNO_NOMEM;
            }

            tmpAreaEnd = tmpAreaEnd->next;
            tmpAreaEnd->beginAddr = findNode->ulHandle;
            tmpAreaEnd->endAddr   = findNode->ulHandle + tmpSize;
            tmpAreaEnd->tmpAddr   = (uintptr_t)NULL;
            tmpAreaEnd->isSysStack = false;
            tmpAreaEnd->next = NULL;
            *areaEnd = tmpAreaEnd;

            /* 防止漏扫此节点作为最后一个扫描节点的情形 */
            if (*nextArea == NULL) {
                *nextArea = tmpAreaEnd;
            }
        }
        /* 从无法访问的节点中删除节点，并释放内存 */
        *memTreeHead = (AVL_NODE_S *)VOS_AVLDel((AVL_NODE_S *)*memTreeHead, findNode, NULL);
        (void)ALLOCATOR_FREE_INNER(findNode);
        findNode = NULL;
    }

    return MEM_HM_OK;
}

MEM_HM_STATIC int32_t MemHMScan(AVL_NODE_S **memTreeHead, MemHMScanArea *areaLinkHead)
{
    int32_t ret;
    uint32_t index;
    uint32_t pageNum;
    uintptr_t length;
    unsigned char *vec;         /* 存放mincore函数的出参数组，判断每个页里是否有物理映射 */
    MemHMScanArea *areaEnd;
    MemHMScanArea *areaTmp;
    MemHMScanArea *nextArea;
    MemHMScanArea *areaLinkEnd;
    uintptr_t startAddr, endAddr, pageStartAddr, pageEndAddr;

    /* 找到链表的结尾 */
    areaEnd = areaLinkHead;
    while (areaEnd->next != NULL) {
        areaEnd = areaEnd->next;
    }

    areaLinkEnd = areaEnd;
    /* 首先检查数据、bss和堆栈段，然后检查memblk */
    areaTmp = areaLinkHead;
    while (areaTmp != NULL) {
        /* 无可达节点 */
        if (*memTreeHead == NULL) {
            break;
        }

        if (areaTmp->beginAddr == 0) {
            areaTmp = areaTmp->next;
            continue;
        }

        startAddr = areaTmp->beginAddr;
        endAddr = areaTmp->endAddr;
        if ((startAddr & ((uintptr_t)(MEM_HM_BYTE_ALIGN - 1))) != 0) {
            startAddr = (startAddr & (~(uintptr_t)(MEM_HM_BYTE_ALIGN - 1))) + MEM_HM_BYTE_ALIGN;
        }

        nextArea = areaTmp->next;
        areaTmp->beginAddr = startAddr;
        length = endAddr - startAddr;

        if ((length >= (uintptr_t)PAGE_SIZE) && (length % PAGE_SIZE == 0)) {
            pageNum = length / PAGE_SIZE;
            vec = (unsigned char *)ALLOCATOR_ALLOC_INNER(pageNum * sizeof(unsigned char));
            if (vec == NULL) {
                return MEM_HM_ERRNO_NOMEM;
            }

            memset(vec, 0, pageNum * sizeof(unsigned char));
            ret = mincore((void *)startAddr, (size_t)length, vec);
            if (ret != MEM_HM_OK) {
                ALLOCATOR_FREE_INNER(vec);
                return MEM_HM_ERRNO_INVAL;
            }

            for (index = 0; index < pageNum; index++) {
                if ((int32_t)vec[index] == 1) {   /* 说明该虚拟地址页被物理映射了，需要进行扫描 */
                    pageStartAddr = startAddr + index * PAGE_SIZE;
                    pageEndAddr = pageStartAddr + PAGE_SIZE - (uintptr_t)(sizeof(uintptr_t) - 1);
                    ret = MemHMScanLoop(pageStartAddr, pageEndAddr, memTreeHead, &areaEnd, &nextArea);
                    if (ret != MEM_HM_OK) {
                        ALLOCATOR_FREE_INNER(vec);
                        return ret;
                    }
                }
            }

            areaTmp = nextArea;
            ALLOCATOR_FREE_INNER(vec);
            continue;
        }

        endAddr = endAddr - (uintptr_t)(sizeof(uintptr_t) - 1);
        ret = MemHMScanLoop(startAddr, endAddr, memTreeHead, &areaEnd, &nextArea);
        if (ret != MEM_HM_OK) {
            return ret;
        }

        areaTmp = nextArea;
    }

    MemHMScanAreaLinkDestory(areaLinkEnd->next);
    areaLinkEnd->next = NULL;

    return MEM_HM_OK;
}

MEM_HM_STATIC int32_t MemHMRepeatMemTreeNodeGet(AVL_NODE_S *node, void *para)
{
    size_t tmpSize;
    AVL_NODE_S tmpNode;
    AVL_NODE_S *newNode;
    AVL_NODE_S *findNode;

    tmpNode.ulHandle = node->ulHandle;
    findNode = VOS_AVLFind((AVL_NODE_S *)para, &tmpNode, NULL);
    if (findNode != NULL) {
        /* 记录此块内存泄露的信息 */
        g_memLeakCount++;
        tmpSize = MemHMUserSizeGet((void *)(findNode->ulHandle));
        g_memLeakSize += tmpSize;
        /* 新增泄露的内存节点 */
        newNode = ALLOCATOR_ALLOC_INNER(sizeof(AVL_NODE_S));
        if (newNode == NULL) {
            return MEM_HM_ERR;
        }

        memset(newNode, 0, sizeof(AVL_NODE_S));
        newNode->ulHandle = tmpNode.ulHandle;
        g_memLeakTreeHead = VOS_AVLAdd(g_memLeakTreeHead, newNode, NULL);
    }

    return MEM_HM_OK;
}

MEM_HM_STATIC AVL_NODE_S *MemHMMemTreeFree(AVL_NODE_S *entry)
{
    if (entry == NULL) {
        return NULL;
    }

    entry->pLeft = MemHMMemTreeFree(entry->pLeft);
    entry->pRight = MemHMMemTreeFree(entry->pRight);
    (void)ALLOCATOR_FREE_INNER(entry);

    return NULL;
}

MEM_HM_STATIC int32_t MemHMLeakMemInfoPrint(AVL_NODE_S *pNode, void *para)
{
    (void)para;
    int32_t index;
    size_t tmpSize;
    void *callerStack;
    void *callerStackTmp;
    char **strings = NULL;

    tmpSize = MemHMUserSizeGet((void *)(pNode->ulHandle));
    MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO,
        " %11d bytes in addr %p are possibly lost. \n", tmpSize, (void *)(pNode->ulHandle));

    callerStack = MemHMCallStackAddrGet((void *)(pNode->ulHandle));
    if (*(uintptr_t *)callerStack == 0xdeaddead) {
        MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO, "             sorry, no stack info.\n");
        return MEM_HM_OK;
    }

    callerStackTmp = *(void **)callerStack;
    strings = backtrace_symbols(&(callerStackTmp), 1);
    if (strings == NULL) {
        MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO, "backtrace_symbols fail!\n");
        return MEM_HM_ERR;
    }

    MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO,
        "             at %-17p %s \n", callerStackTmp, strings[0]);

    free(strings);
    strings = NULL;

    for (index = 1; index < STACK_LEVEL; index++) {
        callerStack = (void *)((uintptr_t)callerStack + sizeof(uintptr_t));
        if (*(uintptr_t *)callerStack == 0xdeaddead) {
            break;
        }

        callerStackTmp = *(void **)callerStack;
        strings = backtrace_symbols(&(callerStackTmp), 1);
        if (strings == NULL) {
            MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO, "backtrace_symbols fail!\n");
            return MEM_HM_ERR;
        }

        MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO,
            "             by %-17p %s \n", callerStackTmp, strings[0]);

        free(strings);
        strings = NULL;
    }

    MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO, "\n");

    return MEM_HM_OK;
}

MEM_HM_STATIC void MemHMLeakTreeWalk(AVL_NODE_S *head, MemHMLeakTreeWalkFunc func, void *para)
{
    if (head == NULL) {
        return;
    }

    MemHMLeakTreeWalk(head->pLeft, func, para);
    (*func)(head, para);
    MemHMLeakTreeWalk(head->pRight, func, para);

    return;
}

MEM_HM_STATIC void MemHMLeakMemInfoShow(void)
{
    void *tmp = NULL;

    MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO,
        "\r\n********************************Leak mem info begin**********************************\n");
    MemHMLeakTreeWalk(g_memLeakTreeHead, MemHMLeakMemInfoPrint, tmp);
    MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO,
        "********************************Leak mem info end************************************\n");
}

MEM_HM_STATIC void MemHMLeakMemSummary(AVL_NODE_S *memTreeHead1, AVL_NODE_S *memTreeHead2)
{
    /* 保留memTreeHead1中节点与memTreeHead2重复的部分到g_memLeakTreeHead树 */
    MemHMLeakTreeWalk(memTreeHead1, MemHMRepeatMemTreeNodeGet, (void *)memTreeHead2);
    if (g_memLeakTreeHead != NULL) {
        /* 表示发生了内存泄露，进行信息打印 */
        MemHMLeakMemInfoShow();
    } else {
        MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO,
            "********************************No memory leaks**********************************\n");
    }
}

void MemHMLeakInfoClean(void)
{
    g_memLeakCount = 0;
    g_memLeakSize = 0;
    MemHMMemTreeFree(g_memLeakTreeHead);
    g_memLeakTreeHead = NULL;
    g_memLeakState = MEM_LEAK_STATE_UNSTART;
}

MEM_HM_STATIC void MemHMLeakScanResourceDestory(MemHMScanArea *area, AVL_NODE_S *tree1, AVL_NODE_S *tree2)
{
    MemHMScanAreaLinkDestory(area);
    (void)MemHMMemTreeFree(tree1);
    (void)MemHMMemTreeFree(tree2);
}

MEM_HM_STATIC int32_t MemHMMemTreeGetAndScan(MemHMScanArea *area, AVL_NODE_S **memTreeHead)
{
    int32_t ret;
    AVL_NODE_S *tmpMemTree = NULL;

    ret = MemHMScanMemTreeGet(&tmpMemTree);
    if (ret != MEM_HM_OK) {
        /* MemHMScanMemTreeGet函数中发生未知失败，需要释放tmpMemTree资源 */
        MemHMMemTreeFree(tmpMemTree);
        return ret;
    }

    /* 对比扫描 */
    ret = MemHMScan(&tmpMemTree, area);
    if (ret != MEM_HM_OK) {
        /* MemHMScan函数中发生未知失败，需要释放tmpMemTree资源 */
        MemHMMemTreeFree(tmpMemTree);
        return ret;
    }

    *memTreeHead = tmpMemTree;
    return MEM_HM_OK;
}

int32_t MemHMLeakScan(void)
{
    int32_t ret;
    uint32_t switchInfo;
    AVL_NODE_S *memTreeHead1 = NULL;
    AVL_NODE_S *memTreeHead2 = NULL;
    MemHMScanArea *areaLinkHead = NULL;

    switchInfo = MemHMSwitchGet();
    if ((switchInfo & MEM_HM_OPTION_WITH_LEAK_CHECK) != MEM_HM_OPTION_WITH_LEAK_CHECK) {
        MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO, "[MemHM-%s]:Leak check switch is close.\n",
            __FUNCTION__);
        return MEM_HM_ERR;
    }

    MemHMLeakInfoClean();

    g_memLeakState = MEM_LEAK_STATE_DOING;

    /* 创建根节点链表，包括.data、.bss和堆栈段 */
    ret = MemHMScanAreaLinkGet(&areaLinkHead);
    if (ret != MEM_HM_OK) {
        g_memLeakState = MEM_LEAK_STATE_UNSTART;
        return ret;
    }

    /* 第一次建立被申请的所有内存的AVL树并进行对比扫描 */
    ret = MemHMMemTreeGetAndScan(areaLinkHead, &memTreeHead1);
    if (ret != MEM_HM_OK) {
        MemHMScanAreaLinkDestory(areaLinkHead);
        g_memLeakState = MEM_LEAK_STATE_UNSTART;
        return ret;
    }

    if (memTreeHead1 == NULL) {
        MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO,
            "********************************No memory leaks**********************************\n");
        MemHMScanAreaLinkDestory(areaLinkHead);
        g_memLeakState = MEM_LEAK_STATE_FINISH;
        return MEM_HM_OK;
    }

    /* delay 1秒钟，减少两颗AVL树重合概率 */
    sleep(1);

    /* 第二次建立被申请的所有内存的AVL树并进行对比扫描 */
    ret = MemHMMemTreeGetAndScan(areaLinkHead, &memTreeHead2);
    if (ret != MEM_HM_OK) {
        MemHMLeakScanResourceDestory(areaLinkHead, memTreeHead1, NULL);
        g_memLeakState = MEM_LEAK_STATE_UNSTART;
        return ret;
    }

    if (memTreeHead2 != NULL) {
        MemHMLeakMemSummary(memTreeHead1, memTreeHead2);
    } else {
        MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO,
            "********************************No memory leaks**********************************\n");
    }

    MemHMLeakScanResourceDestory(areaLinkHead, memTreeHead1, memTreeHead2);
    g_memLeakState = MEM_LEAK_STATE_FINISH;

    return MEM_HM_OK;
}

int32_t MemHMLeakInfoGet(int32_t *memNum, size_t *memSize)
{
    uint32_t switchInfo;

    if ((memNum == NULL) || (memSize == NULL)) {
        return MEM_HM_ERR;
    }

    switchInfo = MemHMSwitchGet();
    if ((switchInfo & MEM_HM_OPTION_WITH_LEAK_CHECK) != MEM_HM_OPTION_WITH_LEAK_CHECK) {
        MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO,
            "[MemHM-%s]:Leak check switch is close.\n", __FUNCTION__);
        return MEM_HM_ERR;
    }

    if (g_memLeakState == MEM_LEAK_STATE_FINISH) {
        *memNum = g_memLeakCount;
        *memSize = g_memLeakSize;
        if (g_memLeakCount != 0) {
            MemHMLeakMemInfoShow();
        } else {
            MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO,
                "********************************No memory leaks**********************************\n");
        }

        return MEM_HM_OK;
    } else if (g_memLeakState == MEM_LEAK_STATE_DOING) {
        return MEM_HM_ERRNO_BUSY;
    } else {
        return MEM_HM_ERRNO_INVAL;
    }
}

#ifdef __cplusplus
}
#endif /* __cpluscplus */
#endif
