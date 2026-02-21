/**
 * @file mem_hm_damagecheck.c
 * @copyright Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * @brief mem_hm_damagecheck 内存健康管理的内存破坏检测功能模块源文件。
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
 * *******************************************************************************************
 */
#ifdef CONFIG_DEBUG_BUILD
#include <unistd.h>
#include <stdlib.h>
#include "mem_hm_inner.h"
#include "mem_hm_memattr.h"
#include "mem_hm_log_inner.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

/**
 * @ingroup mem_hm_damagecheck
 * @brief 内存破坏镜像信息打印函数。
 * @par 描述：内存破坏镜像信息打印函数。
 * @attention
 * @param addr      [IN] 打印的源地址
 * @param preOffset [IN] 源地址往前偏移的长度
 * @param len       [IN] 打印的镜像信息长度
 * @retval 无
 * @par 依赖：无。
 * @see MemHMUserMemDamageCheck、MemHMDfxMemDamageCheck
 */
MEM_HM_STATIC void MemHMDamageInfoShow(uintptr_t addr, size_t preOffset, int32_t len)
{
    char *buffer;
    int32_t i;
    int32_t ret;
    int32_t curLen;
    uint32_t *beginAddr = (uint32_t *)(addr - preOffset);

    if (!MemHMMemAttrRdChk(addr - preOffset, len)) {
        return;
    }

    buffer = (char *)ALLOCATOR_ALLOC_INNER(ECHO_BUFFER_LEN * sizeof(char));
    if (buffer == NULL) {
        return;
    }

    buffer[0] = '\0';
    curLen = 0;
    MemHMLogWrite(MEM_HM_LOG_LEVEL_WARNING, ERRNO_DAMAGE_HAPPEN, "[MemHM-%s]:The meminfo from [%p] to [%p] is:\n",
        __FUNCTION__, beginAddr, (uint32_t *)((uintptr_t)beginAddr + len));

    for (i = 0; i < len; i++) {
        ret = sprintf(buffer + curLen, "0x%08x ", *beginAddr);
        if (ret < 0) {
            ALLOCATOR_FREE_INNER((void *)buffer);
            MemHMLogWrite(MEM_HM_LOG_LEVEL_ERROR, ERRNO_SECUREC_WRONG,
                "[MemHM-%s]:sprintf failed!\n", __FUNCTION__);
            return;
        }
        curLen += ret;
        beginAddr++;
    }

    MemHMLogWrite(MEM_HM_LOG_LEVEL_WARNING, ERRNO_DAMAGE_HAPPEN, "[MemHM-%s]:%s\n", __FUNCTION__, buffer);

    ALLOCATOR_FREE_INNER((void *)buffer);
}

MEM_HM_STATIC int32_t MemHMTailMagicCheck(uintptr_t userAddr, uint32_t *tailAddr)
{
    if ((*tailAddr) != ALLOCATOR_DFX_TAIL_MAGIC) {
        MemHMLogWrite(MEM_HM_LOG_LEVEL_WARNING, ERRNO_DAMAGE_HAPPEN,
            "[MemHM-%s]:Damage Check Report, UserAddr=[%p], TailAddr=[%p], Tail'magic damaged.\n",
            __FUNCTION__, userAddr, tailAddr);
        MemHMDamageInfoShow((uintptr_t)tailAddr, ALLOCATOR_DFX_TAIL_DAMAGE_PRE_SIZE, ALLOCATOR_DFX_DAMAGE_PRINT_SIZE);

        return MEM_HM_ERR;
    }

    return MEM_HM_OK;
}

int32_t MemHMUserMemDamageCheck(void *userAddr)
{
    uint32_t *headAddr;
    uint32_t *tailAddr;
    uint32_t *tmpMagicAddr;
    uint32_t tmpMagic;
    uint32_t tmpMagicAddrValue;
    uint32_t switchInfo;
    size_t size;

    switchInfo = MemHMSwitchGet();
    if ((switchInfo & MEM_HM_OPTION_WITH_DAMAGE_CHECK) != MEM_HM_OPTION_WITH_DAMAGE_CHECK) {
        MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO,
            "[MemHM-%s]:Damage check switch is close.\n", __FUNCTION__);

        return MEM_HM_ERR;
    }

    tmpMagicAddr = (uint32_t *)((uintptr_t)userAddr - sizeof(uint32_t));
    tmpMagicAddrValue = *tmpMagicAddr;
    tmpMagic = tmpMagicAddrValue & ALLOCATOR_DFX_CHECK_MASK;

    if ((tmpMagic != ALLOCATOR_DFX_STACK_END_MAGIC) && (tmpMagic != ALLOCATOR_DFX_OFFSET_MAGIC)) {
        MemHMLogWrite(MEM_HM_LOG_LEVEL_WARNING, ERRNO_DAMAGE_HAPPEN,
            "[MemHM-%s]:Damage Check Report, UserAddr=[%p], Head'tailMagic damaged.\n", __FUNCTION__, userAddr);
        MemHMDamageInfoShow((uintptr_t)userAddr, ALLOCATOR_DFX_USER_DAMAGE_PRE_SIZE, ALLOCATOR_DFX_DAMAGE_PRINT_SIZE);
        if (g_himalloc_enable_mcheck) {
            abort(); /* 内存破坏了，可能是非法地址，在MALLOC_CHECK_打开的情况下abort */
        }

        return MEM_HM_ERR;
    }

    if (tmpMagic == ALLOCATOR_DFX_OFFSET_MAGIC) {
        headAddr = (uint32_t *)((uintptr_t)userAddr - ((tmpMagicAddrValue & ~ALLOCATOR_DFX_OFFSET_MAGIC) >> SHIFT_BIT));
    } else {
        headAddr = (uint32_t *)((uintptr_t)userAddr - ALLOCATOR_DFX_HEAD_SIZE);
    }

    size = *(size_t *)((uintptr_t)headAddr + sizeof(uint32_t));
    tailAddr = (uint32_t *)((uintptr_t)userAddr + size);

    if (((*headAddr) & ALLOCATOR_DFX_CHECK_MASK) != ALLOCATOR_DFX_HEAD_OFFSET_MAGIC) {
        MemHMLogWrite(MEM_HM_LOG_LEVEL_WARNING, ERRNO_DAMAGE_HAPPEN,
            "[MemHM-%s]:Damage Check Report, UserAddr=[%p], HeadAddr=[%p], Head'headMagic damaged.\n",
            __FUNCTION__, userAddr, headAddr);
        MemHMDamageInfoShow((uintptr_t)headAddr, ALLOCATOR_DFX_HEAD_DAMAGE_PRE_SIZE, ALLOCATOR_DFX_DAMAGE_PRINT_SIZE);
        if (g_himalloc_enable_mcheck) {
            abort(); /* 内存破坏了，可能是double free，在MALLOC_CHECK_打开的情况下abort */
        }

        return MEM_HM_ERR;
    }

    if (MemHMTailMagicCheck((uintptr_t)userAddr, tailAddr) != MEM_HM_OK) {
        return MEM_HM_ERR;
    }

    return MEM_HM_OK;
}

int32_t MemHMDfxMemDamageCheck(uint32_t *dfxAddr)
{
    uintptr_t userAddr;
    uint32_t *tailAddr;
    uint32_t dfxAddrValue;
    uint32_t tmpMagic;
    uint32_t switchInfo;
    size_t size;

    switchInfo = MemHMSwitchGet();
    if ((switchInfo & MEM_HM_OPTION_WITH_DAMAGE_CHECK) != MEM_HM_OPTION_WITH_DAMAGE_CHECK) {
        MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO,
            "[MemHM-%s]:Damage check switch is close.\n", __FUNCTION__);

        return MEM_HM_ERR;
    }

    dfxAddrValue = *dfxAddr;

    if ((dfxAddrValue & ALLOCATOR_DFX_CHECK_MASK) != ALLOCATOR_DFX_HEAD_OFFSET_MAGIC) {
        MemHMLogWrite(MEM_HM_LOG_LEVEL_WARNING, ERRNO_DAMAGE_HAPPEN,
            "[MemHM-%s]:Damage Check Report, HeadAddr=[%p], Head'headMagic damaged.\n", __FUNCTION__, dfxAddr);
        MemHMDamageInfoShow((uintptr_t)dfxAddr, ALLOCATOR_DFX_HEAD_DAMAGE_PRE_SIZE, ALLOCATOR_DFX_DAMAGE_PRINT_SIZE);

        return MEM_HM_ERR;
    }

    if (((dfxAddrValue & ~ALLOCATOR_DFX_HEAD_OFFSET_MAGIC) >> SHIFT_BIT) !=
        ((ALLOCATOR_DFX_HEAD_MAGIC & ~ALLOCATOR_DFX_HEAD_OFFSET_MAGIC) >> SHIFT_BIT)) {
        userAddr = (uintptr_t)dfxAddr + ((dfxAddrValue & ~ALLOCATOR_DFX_HEAD_OFFSET_MAGIC) >> SHIFT_BIT);
    } else {
        userAddr = (uintptr_t)dfxAddr + ALLOCATOR_DFX_HEAD_SIZE;
    }

    size = *(size_t *)((uintptr_t)dfxAddr + sizeof(uint32_t));
    tailAddr = (uint32_t *)(userAddr + size);
    tmpMagic = *(uint32_t *)(userAddr - sizeof(uint32_t)) & ALLOCATOR_DFX_CHECK_MASK;

    if ((tmpMagic != ALLOCATOR_DFX_STACK_END_MAGIC) && (tmpMagic != ALLOCATOR_DFX_OFFSET_MAGIC)) {
        MemHMLogWrite(MEM_HM_LOG_LEVEL_WARNING, ERRNO_DAMAGE_HAPPEN,
            "[MemHM-%s]:Damage Check Report, UserAddr=[%p], HeadAddr=[%p], Head'tailMagic damaged.\n",
            __FUNCTION__, userAddr, dfxAddr);
        MemHMDamageInfoShow(userAddr, ALLOCATOR_DFX_USER_DAMAGE_PRE_SIZE, ALLOCATOR_DFX_DAMAGE_PRINT_SIZE);

        return MEM_HM_ERR;
    }

    if (MemHMTailMagicCheck(userAddr, tailAddr) != MEM_HM_OK) {
        return MEM_HM_ERR;
    }

    return MEM_HM_OK;
}

/**
 * @ingroup mem_hm_damagecheck
 * @brief 对遍历到的所有内存进行扫描并记录最终是否被破坏的结果
 * @par 描述：对遍历到的所有内存进行扫描并记录最终是否被破坏的结果。
 * @attention
 * @param addr [IN] 内存块的起始地址
 * @param para [IN] 记录最终是否被破坏的结果
 * @retval MEM_HM_OK 检测成功； MEM_HM_ERR 检测失败
 * @par 依赖：无。
 * @see MemHMDfxMemDamageCheck
 */
MEM_HM_STATIC int32_t MemHMBatchMemDamageCheckTraverseFunc(void *addr, size_t size, void *para)
{
    int32_t ret;
    (void)size;

    ret = MemHMDfxMemDamageCheck(addr);
    if (ret != MEM_HM_OK) {
        *(int32_t *)para = 1;
    }

    return MEM_HM_OK;
}

int32_t MemHMBatchMemDamageCheck(void)
{
    uint32_t switchInfo;
    int32_t ret;
    int32_t isDamageHappened = 0;

    switchInfo = MemHMSwitchGet();
    if ((switchInfo & MEM_HM_OPTION_WITH_DAMAGE_CHECK) != MEM_HM_OPTION_WITH_DAMAGE_CHECK) {
        MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO,
            "[MemHM-%s]:Damage check switch is close.\n", __FUNCTION__);

        return MEM_HM_ERR;
    }

    ret = ALLOCATOR_ALL_MEM_TRAVERSE(&isDamageHappened, MemHMBatchMemDamageCheckTraverseFunc, false);
    if (ret != MEM_HM_OK) {
        return MEM_HM_ERR;
    } else {
        if (isDamageHappened == 0) {
            MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO, "[MemHM-%s]:Nothing damaged.\n", __FUNCTION__);
        } else {
            MemHMLogWrite(MEM_HM_LOG_LEVEL_INFO, NORMAL_NO, "[MemHM-%s]:Something damaged.\n",
                __FUNCTION__);
        }
        return MEM_HM_OK;
    }
}

#ifdef __cplusplus
}
#endif /* __cpluscplus */
#endif
