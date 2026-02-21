/**
 * @file mem_hm_memattr.h
 * @copyright Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * @brief mem_hm_memattr header file
 * @details mem_hm_memattr header file
 * @author Anonymous
 * @date 2022-11-07
 * @version v0.0.1
 * *******************************************************************************************
 * @par Modify log:
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2022-11-07  <td>0.0.1    <td>Anonymous   <td>Initial Version
 * </table>
 * *******************************************************************************************
 */
#ifndef MEM_HM_MEMATTR_H
#define MEM_HM_MEMATTR_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

bool MemHMMemAttrRdChk(uintptr_t chkAddr, int32_t len);

#ifdef __cplusplus
}
#endif /* __cpluscplus */

#endif