/**
 * @file himalloc_mallinfo.c
 * @copyright Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * @brief himalloc mallinfo 源文件。
 * @details
 * @author anonymous
 * @date 2022-03-08
 * @version v1.0.0
 * *******************************************************************************************
 * @par 修改日志：
 * <table>
 * <tr><th>Date        <th>Version  <th>Author    <th>Description
 * <tr><td>2022-03-08  <td>1.0.0    <td>anonymous <td>创建初始版本
 * </table>
 *
 * *******************************************************************************************
 */
#include <malloc.h>
#include "himalloc_inner.h"

struct mallinfo himalloc_mallinfo(void)
{
    struct mallinfo m;

    m.arena = 0UL;
    m.hblks = 0UL;
    m.smblks = 0UL;
    m.hblkhd = 0UL;
    m.fsmblks = 0UL;
    m.ordblks = 0UL;
    m.usmblks = 0UL;
    m.fordblks = 0UL;
    m.keepcost = 0UL;
    m.uordblks = 0UL;

    return m;
}

WEAK_ALIAS(himalloc_mallinfo, mallinfo);
