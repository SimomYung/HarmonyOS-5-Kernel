/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
 *
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef BSP_MDM_STR_H
#define BSP_MDM_STR_H

/**
 * @brief 描述注册到MDM STR框架的模块
 * @attention
 * <ul><li>次枚举只能用于相等判断，用户不应该依赖此枚举的排列顺序，枚举号大小</li></ul>
 */
typedef enum {
    STR_MODULE_MSG = 0,
    STR_MODULE_FIPC = 1,
    STR_MODULE_IPC = 2,
    STR_MODULE_DIAG = 3,
    STR_MODULE_TESET,
    STR_MODULE_MAX,
} mdm_str_module_e;

/**
 * @brief 描述MDM STR框架处于阶段
 * @attention
 * <ul><li>次枚举只能用于相等判断，用户不应该依赖此枚举的排列顺序，枚举号大小</li></ul>
 */
typedef enum {
    PHASE_ENTER_MDM_STR = 0,
    PHASE_EXIT_MDM_STR,
    PROC_PHASE_MAX,
} mdm_str_proc_phase_e;

/**
 * @brief 描述注册到MDM STR框架回调函数
 */
typedef int (*mdm_str_proc_func)(mdm_str_proc_phase_e proc_phase);

/**
 * @brief 注册到mdm str框架接口
 *
 * @par 描述:
 * 注册到mdm str框架的接口，用于mdm str流程调用涉及模块的相关处理
 *
 * @param[in]  module, 描述注册到MDM STR框架的模块，使用mdm_str_module_e
 * @param[in]  func, 描述注册到MDM STR框架回调函数
 *
 * @retval 0,注册成功
 * @retval 其他, 执行失败
 */
int bsp_mdm_str_register(mdm_str_module_e module, mdm_str_proc_func func);

/**
 * @brief mdm str状态获取
 *
 * @par 描述:
 * 用于给获取是否处于mdm str状态
 *
 * @retval 参照mdm_str_proc_phase_e
 *
 */
int bsp_mdm_str_get_status(void);

/**
 * @brief modem单独复位处理前mdm str处理接口
 *
 * @par 描述:
 * modem单独复位处理前mdm str处理接口
 *
 * @retval 0 处理成功
 * @retval 1 处于STR状态
 * @retval 其他, 执行失败
 */
int bsp_mdm_str_before_mdm_reset(void);

/**
 * @brief modem单独复位完成后mdm str处理接口
 *
 * @par 描述:
 * modem单独复位完成后mdm str处理接口
 *
 * @retval 0 处理成功
 * @retval 其他, 执行失败
 */
int bsp_mdm_str_after_mdm_reset(void);

/**
 * @brief 描述省电模式流程所处状态
 * @attention
 * <ul><li>枚举只能用于相等判断，用户不应该依赖此枚举的排列顺序，枚举号大小</li></ul>
 */
typedef enum {
    PSMC_ENTERING, /**< 正在进入省电模式 */
    PSMC_ACTIVE,   /**< 已进入省电模式  */
    PSMC_EXITING,  /**< 正在退出省电模式 */
    PSMC_INACTIVE  /**< 已退出省电模式  */
} psmc_status_e;

/**
 * @brief 获取省电模式状态
 *
 * @par 描述:
 * 用于获取省电模式状态
 *
 * @retval psmc_status_e
 *
 */
psmc_status_e bsp_psmc_get_status(void);
#endif