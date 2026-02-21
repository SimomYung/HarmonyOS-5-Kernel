/**
 * @file himalloc_hook.c
 * @copyright Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * @brief himalloc hook 源文件。
 * @details
 * @author anonymous
 * @date 2021-12-09
 * @version v1.0.0
 * *******************************************************************************************
 * @par 修改日志：
 * <table>
 * <tr><th>Date        <th>Version  <th>Author    <th>Description
 * <tr><td>2021-12-09  <td>1.0.0    <td>anonymous <td>创建初始版本
 * </table>
 *
 * *******************************************************************************************
 */
#include <stdint.h>
#include "vos_himallocarena.h"
#include "vos_himalloctcache.h"
#include "vos_himalloctlb.h"
#include "vos_himallocgrow.h"
#include "vos_memarea.h"
#include "vos_metaalloc.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

static void himalloc_lock_all(void)
{
    /* 实现将himalloc内所有互斥锁上锁，保证进程fork功能正常 */
    VOS_MEM_AREA_S *pstArea;
    VOS_RTREE_ROOT_S *rTreeRoot;
    VOS_HIMALLOC_ARENA_S *pstArena;

    pstArena = VosHimallocArenaGet(0); /* 0 顾名思义为0号分区，鸿蒙OS仅只用一个分区，即0号分区 */
    /* 首先锁himalloc的线程缓存锁 */
    (void)OSAL_LockLock(&(pstArena->stTcacheLock));
    /* 其次锁himalloc的slice块锁 */
    (void)VosHimallocLock(&(pstArena->stSliceHeap.astBinsLock[0])); /* 0 目前是因为所有slice类型共用一把锁 */
    /* 再次锁himalloc的block块锁 */
    (void)VosHimallocLock(&(pstArena->stBlockHeap.stTreeBinLock));
    /* 接着锁himalloc的radixTree锁 */
    rTreeRoot = (VOS_RTREE_ROOT_S *)VosHimallocRtreeIdGet();
    OSAL_LockLock(&(rTreeRoot->stLock));
    /* 然后锁himalloc的area锁 */
    pstArea = (VOS_MEM_AREA_S *)g_HimallocAreaId;
    OSAL_LockLock(&(pstArea->stLock));
    /* 最后锁himalloc的meta锁 */
    OSAL_LockLock(&(g_stMetaMem.stLock));
}

static void himalloc_unlock_all(void)
{
    /* 实现将himalloc内所有互斥锁解锁，保证进程fork功能正常 */
    const uint32_t ret = HIMALLOC_OK;
    VOS_MEM_AREA_S *pstArea;
    VOS_RTREE_ROOT_S *rTreeRoot;
    VOS_HIMALLOC_ARENA_S *pstArena;

    pstArena = VosHimallocArenaGet(0); /* 0 顾名思义为0号分区，鸿蒙OS仅只用一个分区，即0号分区 */
    /* 首先解锁himalloc的meta锁 */
    OSAL_LockUnLock(&(g_stMetaMem.stLock), ret);
    /* 其次解锁himalloc的area锁 */
    pstArea = (VOS_MEM_AREA_S *)g_HimallocAreaId;
    OSAL_LockUnLock(&(pstArea->stLock), ret);
    /* 再次解锁himalloc的radixTree锁 */
    rTreeRoot = (VOS_RTREE_ROOT_S *)VosHimallocRtreeIdGet();
    OSAL_LockUnLock(&(rTreeRoot->stLock), ret);
    /* 接着解锁himalloc的block块锁 */
    VosHimallocUnLock(&(pstArena->stBlockHeap.stTreeBinLock));
    /* 然后解锁himalloc的slice块锁 */
    VosHimallocUnLock(&(pstArena->stSliceHeap.astBinsLock[0])); /* 0 目前是因为所有slice类型共用一把锁 */
    /* 最后解锁himalloc的线程缓存锁 */
    OSAL_LockUnLock(&(pstArena->stTcacheLock), ret);
    (void)ret;
}

/* called by parent process before fork */
void malloc_lock_parent(void)
{
    himalloc_lock_all();
}

/* called by parent process after fork */
void malloc_unlock_parent(void)
{
    himalloc_unlock_all();
}

/* called by child process after fork */
void malloc_unlock_child(void)
{
    himalloc_unlock_all();
}

void malloc_reclaim_thread_memory(long long addr)
{
    VosHimallocThreadDestroy(addr);
}

#ifdef __cplusplus
}
#endif /* __cpluscplus */
