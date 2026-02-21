/**
 * @file mem_hm_statistic.c
 * @copyright Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * @brief mem_hm_statistic 内存健康管理的内存统计功能模块源文件。
 * @details
 * @author anonymous
 * @date 2021-03-11
 * @version v1.0.0
 * *******************************************************************************************
 * @par 修改日志：
 * <table>
 * <tr><th>Date        <th>Version  <th>Author    <th>Description
 * <tr><td>2021-03-11  <td>1.0.0    <td>anonymous <td>创建初始版本
 * </table>
 *
 * *******************************************************************************************
 */
#ifdef CONFIG_DEBUG_BUILD
#include <execinfo.h>
#include <stdlib.h>
#include <unistd.h>
#include "mem_hm_inner.h"
#include "cstl_list.h"
#include "mem_hm_log_inner.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

/* 单个内存调用点统计结果结构体 */
typedef struct TagMemBlockInfo {
    void *retAddr;       /**< 调用者地址 */
    size_t memSize;      /**< 该内存调用点当前申请内存的总大小 */
    uint32_t num;        /**< 该内存调用点当前申请内存的个数 */
    uint32_t reserved;   /**< 字节对齐，保留字段用于后续扩展 */
} MemBlockInfo;

/* 总体内存统计结果结构体 */
typedef struct TagMemUsedInfo {
    uint32_t totalNum;        /**< 进程使用himalloc的内存总个数 */
    uint32_t totalBrokenNum;  /**< 进程使用himalloc的内存中被破坏的个数 */
    size_t totalMemSize;      /**< 进程使用himalloc申请的内存总大小 */
    CstlList *listPointer;    /**< 进程使用himalloc申请的内存调用点链表 */
} MemUsedInfo;

/**
 * @ingroup mem_hm_statistic
 * @brief 对遍历到的所有内存进行统计并更新统计信息。
 * @par 描述：对遍历到的所有内存进行统计并更新统计信息。
 * @attention
 * @param addr [IN] 内存块的起始地址
 * @param para [IN] 统计信息结构体指针
 * @retval MEM_HM_OK 统计成功； MEM_HM_ERR 统计失败
 * @par 依赖：无。
 * @see MemHMAllInfoShow
 */
MEM_HM_STATIC int32_t MemHMAllMemStatisticWalkFunc(void *addr, size_t mem_size, void *para)
{
    MemUsedInfo *memUsedInfo = para;
    int32_t ret;
    bool isExist = false;
    MemBlockInfo *memEntry;
    size_t size;
    void *retAddr;
    CstlListIterator it;
    CstlList *listPointer = memUsedInfo->listPointer;
    (void)mem_size;

    memUsedInfo->totalNum++;

    ret = MemHMDfxMemDamageCheck((uint32_t *)addr);
    if (ret != MEM_HM_OK) {
        memUsedInfo->totalBrokenNum++;
        return MEM_HM_OK;
    }

    size = *(size_t *)((uintptr_t)addr + sizeof(uint32_t));
    memUsedInfo->totalMemSize += size;
    retAddr = *(char **)((uintptr_t)addr + ALLOCATOR_DFX_HEAD2RETADDR_SIZE);

    for (it = CstlListIterBegin(listPointer); it != CstlListIterEnd(listPointer);
        it = CstlListIterNext(listPointer, it)) {
        memEntry = (MemBlockInfo *)CstlListIterData(it);
        if (memEntry->retAddr == retAddr) {
            memEntry->num++;
            memEntry->memSize += size;
            isExist = true;
            break;
        }
    }

    if (!isExist) {
        memEntry = (MemBlockInfo *)ALLOCATOR_ALLOC_INNER(sizeof(MemBlockInfo));
        if (memEntry == NULL) {
            return MEM_HM_ERR;
        }
        memEntry->retAddr = retAddr;
        memEntry->num = 1;
        memEntry->memSize = size;
        CstlListPushBack(listPointer, (uintptr_t)memEntry, sizeof(MemBlockInfo));
    }

    return MEM_HM_OK;
}

/**
 * @ingroup mem_hm_statistic
 * @brief 对遍历到的所有内存进行详细的信息展示。
 * @par 描述：对遍历到的所有内存进行详细的信息展示。
 * @attention
 * @param addr [IN] 内存块的起始地址
 * @param para [IN] 统计信息结构体指针
 * @retval MEM_HM_OK 统计成功； MEM_HM_ERR 统计失败
 * @par 依赖：无。
 * @see MemHMDetailInfoShow
 */
MEM_HM_STATIC int32_t MemHMAllMemDetailWalkFunc(void *addr, size_t mem_size, void *para)
{
    size_t size;
    void *userAddr = NULL, *stackAddr = NULL, *stackAddrTmp = NULL;
    char **strings = NULL;
    (void)para;
    (void)mem_size;

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

    size = *(size_t *)((uintptr_t)addr + sizeof(uint32_t));
    stackAddr = (void *)((uintptr_t)addr + sizeof(uint32_t) + sizeof(size_t));

    MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO,
        "* %11d bytes in addr %p, stack info:\n", size, userAddr);
    if (*(uintptr_t *)stackAddr == 0xdeaddead) {
        MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO, "* sorry, no stack info.\n");
        MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO,
            "--------------------------------------------------------------------------------\n");
        return MEM_HM_OK;
    }

    stackAddrTmp = *(void **)stackAddr;
    strings = backtrace_symbols(&(stackAddrTmp), 1);
    if (strings == NULL) {
        MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO, "backtrace_symbols fail!\n");
    } else {
        MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO, "*              at %-17p %s \n", stackAddrTmp, strings[0]);
        free(strings);
        strings = NULL;
    }

    for (int32_t index = 1; index < STACK_LEVEL; index++) {
        stackAddr = (void *)((uintptr_t)stackAddr + sizeof(uintptr_t));
        if (*(uintptr_t *)stackAddr == 0xdeaddead) {
            break;
        }

        stackAddrTmp = *(void **)stackAddr;
        strings = backtrace_symbols(&(stackAddrTmp), 1);
        if (strings == NULL) {
            MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO, "backtrace_symbols fail!\n");
        } else {
            MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO, "*             by %-17p %s \n", stackAddrTmp, strings[0]);
            free(strings);
            strings = NULL;
        }
    }

    MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO,
        "--------------------------------------------------------------------------------\n");

    return MEM_HM_OK;
}

/* 显示内存信息展示接口的帮助信息 */
MEM_HM_STATIC inline void MemHMHelpInfoShow(void)
{
    MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO,
        "\r\n********************************************************************************");
    MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO, "\r\n show memory usage help of parameter:");
    MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO, "\r\n ( 0 ): show help info.");
    MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO, "\r\n ( 1 ): show all mem used info.");
    MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO, "\r\n ( 2 ): show detail mem used info.");
    MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO,
        "\r\n********************************************************************************");
    MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO, "\r\n");
}

MEM_HM_STATIC inline void MemHMAllInfoTitleShow(void)
{
    MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO,
        "\r\n********************************************************************************");
    MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO,
        "\r\n %-44s |     %-12s %-13s ", "functionName", "num", "usedMemory(B)");
    MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO,
        "\r\n********************************************************************************");
}

MEM_HM_STATIC void MemHMAllInfoTotalShow(const MemUsedInfo *memUsedInfo)
{
    MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO,
        "\r\n--------------------------------------------------------------------------------");

    if (memUsedInfo->totalBrokenNum != 0) {
        MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO,
            "\r\n brokenNum: %-40s %-26d ", " ", memUsedInfo->totalBrokenNum);
    }

    MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO,
        "\r\n totalNum : %-40s %-12d %-13d ", " ", memUsedInfo->totalNum,
        memUsedInfo->totalMemSize);
    MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO,
        "\r\n********************************************************************************\r\n");
}

MEM_HM_STATIC void MemHMDetailInfoTitleShow(void)
{
    MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO,
        "\r\n********************************************************************************");
    MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO,
        "\r\n**                     Himalloc all mem detail info show                      **");
    MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO,
        "\r\n********************************************************************************\r\n");
}

MEM_HM_STATIC void MemHMDetailInfoTailShow(void)
{
    MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO,
        "\r\n**                  Himalloc all mem detail info show finish                  **");
    MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO,
        "\r\n********************************************************************************\r\n");
}

/* 显示所有内存调用点的详细信息，逐个输出 */
MEM_HM_STATIC void MemHMDetailInfoShow(void)
{
    int32_t ret;
    int32_t tmp = 0;

    MemHMDetailInfoTitleShow();

    ret = ALLOCATOR_ALL_MEM_TRAVERSE((void *)&tmp, MemHMAllMemDetailWalkFunc, false);
    if (ret != MEM_HM_OK) {
        MemHMLogWrite(MEM_HM_LOG_LEVEL_ERROR, ERRNO_TRAVERSE_WRONG, "[MemHM-%s]:traverse fail!\n", __FUNCTION__);
        return;
    }

    MemHMDetailInfoTailShow();
}

/* 显示所有内存调用点的统计信息 */
MEM_HM_STATIC void MemHMAllInfoShow(void)
{
    int32_t ret;
    MemBlockInfo *memEntry;
    CstlListIterator it;
    CstlList *listPointer;
    char **strings;

    listPointer = (CstlList *)ALLOCATOR_ALLOC_INNER(sizeof(CstlList));
    if (listPointer == NULL) {
        MemHMLogWrite(MEM_HM_LOG_LEVEL_ERROR, ERRNO_ALLOC_WRONG, "[MemHM-%s]:Alloc mem fail!\n", __FUNCTION__);
        return;
    }

    CstlListInit(listPointer, NULL);

    MemUsedInfo *memUsedInfo = (MemUsedInfo *)ALLOCATOR_ALLOC_INNER(sizeof(MemUsedInfo));
    if (memUsedInfo == NULL) {
        MemHMLogWrite(MEM_HM_LOG_LEVEL_ERROR, ERRNO_ALLOC_WRONG, "[MemHM-%s]:Alloc mem fail!\n", __FUNCTION__);
        CstlListDeinit(listPointer);
        ALLOCATOR_FREE_INNER(listPointer);

        return;
    }

    memUsedInfo->totalNum = 0;
    memUsedInfo->totalMemSize = 0;
    memUsedInfo->totalBrokenNum = 0;
    memUsedInfo->listPointer = listPointer;

    ret = ALLOCATOR_ALL_MEM_TRAVERSE((void *)memUsedInfo, MemHMAllMemStatisticWalkFunc, false);
    if (ret != MEM_HM_OK) {
        CstlListDeinit(listPointer);
        ALLOCATOR_FREE_INNER(listPointer);
        ALLOCATOR_FREE_INNER(memUsedInfo);

        return;
    }

    MemHMAllInfoTitleShow();

    for (it = CstlListIterBegin(listPointer); it != CstlListIterEnd(listPointer);) {
        memEntry = (MemBlockInfo *)CstlListIterData(it);
        /* 根据地址获取函数名，如果srings为空，则获取失败，1表示只获取一个地址 */
        strings = backtrace_symbols (&(memEntry->retAddr), 1);
        if (strings != NULL) {
            MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO, "\r\n %-44s |     %-12d %-13d ",
                strings[0], memEntry->num, memEntry->memSize);
            free (strings);
            strings = NULL;
        }

        ALLOCATOR_FREE_INNER(memEntry);
        it = CstlListIterErase(listPointer, it);
    }

    MemHMAllInfoTotalShow(memUsedInfo);

    CstlListDeinit(listPointer);
    ALLOCATOR_FREE_INNER(listPointer);
    ALLOCATOR_FREE_INNER(memUsedInfo);
}

void MemHMStatisticsShow(uint32_t option)
{
    uint32_t switchInfo;

    switchInfo = MemHMSwitchGet();
    if ((switchInfo & MEM_HM_OPTION_WITH_STATISTIC) != MEM_HM_OPTION_WITH_STATISTIC) {
        MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO,
            "[MemHM-%s]:Statistic switch is close.\n", __FUNCTION__);

        return;
    }

    switch (option) {
        /* option:1 显示所有内存调用点的统计信息 */
        case 1:
            MemHMAllInfoShow();
            break;
        /* option:2 显示所有内存调用点的详细信息，逐个输出 */
        case 2:
            MemHMDetailInfoShow();
            break;
        /* default 显示内存信息展示接口的帮助信息 */
        default:
            MemHMHelpInfoShow();
            break;
    }
}

#ifdef __cplusplus
}
#endif /* __cpluscplus */
#endif
