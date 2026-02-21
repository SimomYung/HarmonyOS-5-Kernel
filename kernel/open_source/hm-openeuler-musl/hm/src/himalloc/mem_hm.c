/**
 * @file mem_hm.c
 * @copyright Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * @brief mem_hm 内存健康管理公共模块，负责内存破坏、泄露、统计等子模块的开关、日志打印等。
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
#include "mem_hm_inner.h"
#include "mem_log.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

uint32_t g_memHMSwitchInfo;

char *g_memHMlogNames[MEM_HM_LOG_LEVEL_BUTT] = { "LOG-INFO", "LOG-WARNING", "LOG-ERROR", "LOG-EXCEPT" };

void MemHMSwitchSet(uint32_t option)
{
    uint32_t switchInfo = 0;

    if ((option & MEM_HM_OPTION_WITH_DAMAGE_CHECK) == MEM_HM_OPTION_WITH_DAMAGE_CHECK) {
        switchInfo |= MEM_HM_OPTION_WITH_DAMAGE_CHECK;
    }

    if ((option & MEM_HM_OPTION_WITH_STATISTIC) == MEM_HM_OPTION_WITH_STATISTIC) {
        switchInfo |= MEM_HM_OPTION_WITH_STATISTIC;
    }

    if ((option & MEM_HM_OPTION_WITH_LEAK_CHECK) == MEM_HM_OPTION_WITH_LEAK_CHECK) {
        switchInfo |= MEM_HM_OPTION_WITH_LEAK_CHECK;
    }

    g_memHMSwitchInfo = switchInfo;
}

uint32_t MemHMSwitchGet(void)
{
    return g_memHMSwitchInfo;
}

void __attribute__((constructor)) MemHMInit(void)
{
    g_memHMSwitchInfo = MEM_HM_OPTION_WITH_DAMAGE_CHECK | MEM_HM_OPTION_WITH_STATISTIC | MEM_HM_OPTION_WITH_LEAK_CHECK;
}

#ifdef __cplusplus
}
#endif /* __cpluscplus */
#endif
