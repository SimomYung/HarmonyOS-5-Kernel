/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
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

/**
 * @brief   飞龙架构2.0 pcie模块对外头文件
 * @file    mdrv_bmi_pcie.h
 * @version v1.0
 * @date    2023.08.04
 * @note    该文件增删改需通过接口变更评审
 * <ul><li>v1.0|2023.08.04|创建文件</li></ul>
 * @since
 */

#ifndef _MDRV_BMI_PCIE_H_
#define _MDRV_BMI_PCIE_H_

/**
 * @brief pcie参考时钟输出状态配置
 *
 * @par 描述:
 * 用于控制pcie参考时钟输出状态
 *
 * @param[in]  id, 表示需要配置的pcie (0，1...) for (PCIE0, PCIE1...)
 * @param[in]  enable,  传入非0值表示PCIE IO输出参考时钟，0表示PCIE IO停止输出参考时钟
 *
 * @retval 0表示配置成功，非0表示配置失败
 */
int mdrv_pcie_refclk_output(unsigned int id, unsigned int enable);

/**
 * @brief pcie系统关钟下电函数
 *
 * @par 描述:
 * 用于控制pcie系统关钟下电
 *
 * @retval 0表示配置成功，非0表示配置失败
 *
 * @attention
 * <ul><li>1、接口暂不支持并发场景，防止重复下电</li></ul>
 * <ul><li>2、需要和mdrv_iot_pcie_pm_sys_resume接口配对使用，先调用mdrv_iot_pcie_pm_sys_suspend，再调用mdrv_iot_pcie_pm_sys_resume</li></ul>
 * <ul><li>3、接口暂不支持重入场景，同时再次调用前需要保证已调用过mdrv_iot_pcie_pm_sys_resume接口</li></ul>
 * <ul><li>4、调用过程中需要持wakelock，保证调用suspend和resume过程系统不进低功耗流程</li></ul>
 */
int mdrv_iot_pcie_pm_sys_suspend(void);

/**
 * @brief pcie系统上电开钟函数
 *
 * @par 描述:
 * 用于控制pcie系统上电开钟
 *
 * @retval 0表示配置成功，非0表示配置失败
 *
 * @attention
 * <ul><li>1、接口暂不支持并发场景，防止重复下电</li></ul>
 * <ul><li>2、需要和mdrv_iot_pcie_pm_sys_suspend接口配对使用，先调用mdrv_iot_pcie_pm_sys_suspend，再调用mdrv_iot_pcie_pm_sys_resume</li></ul>
 * <ul><li>3、接口暂不支持重入场景，同时再次调用前需要保证已调用过mdrv_iot_pcie_pm_sys_suspend接口</li></ul>
 * <ul><li>4、调用过程中需要持wakelock，保证调用suspend和resume过程系统不进低功耗流程</li></ul>
 */
int mdrv_iot_pcie_pm_sys_resume(void);

/**
 * @brief pcie控制器连接状态查询接口
 *
 * @par 描述:
 * 用于查询pcie控制器连接状态
 *
 * @param[in]  id, 表示需要配置的pcie (0，1...) for (PCIE0, PCIE1...)
 * @retval 0表示配置成功，非0表示配置失败
 */
int mdrv_pcie_is_linked_up(u32 id);

#endif
